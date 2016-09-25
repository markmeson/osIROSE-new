def input_int(string):
    try:
        return int(input(string))
    except ValueError:
        return 0

def menu():
    print("---Menu---")
    print("1 - input simple type")
    print("2 - create complex type (not working)")
    print("3 - finish")
    value = input_int("> ")
    while value < 1 and value > 3:
        value = input_int("> ")
    return value

class Class:
    def __init__(self, name, ePacketType):
        self.name = name
        self.ePacketType = ePacketType
        self.isReceived = "PAKCS_" in ePacketType
        self.constructor = ""
        self.variables = []

    def addType(self, varType, varName):
        if varName[-1] != '_':
            varName += '_'
        self.variables.append((varType, varName))

    def header(self):
        if self.isReceived:
            self.constructor = "{0}(uint8_t buffer[MAX_PACKET_SIZE]);"
        else:
            self.constructor = "{0}(".format(self.name)
            for var in self.variables:
                self.constructor += "{} {}, ".format(var[0], var[1][:-1])
            self.constructor = self.constructor[:-2]
            self.constructor += ");"
        data = ("class {0} : public CRosePacket {{\n\tpublic:\n\t\t{0}();\n\n\t" + self.constructor + "\n\t\tvirtual ~{0}();\n").format(self.name)
        if self.isReceived:
            for var in self.variables:
                data += "\n\t\t"
                data += "{} {}() const {{ return {}; }}".format(var[0], var[1].translate({ord('_') : None}), var[1])
        else:
            data += "\n\n\tprotected:\n\t\tvirtual void pack();"
        data += "\n\n\tprivate:"
        for var in self.variables:
            data += "\n\t\t"
            data += "{} {};".format(var[0], var[1])
        data += "\n};"
        return data

    def cpp(self):
        data = "RoseCommon::{0}::{0}() : CRosePacket(ePacketType::{1}) {{}}".format(self.name, self.ePacketType)
        data += ("\n\nRoseCommon::{0}::" + self.constructor[:-1] + " : CRosePacket(ePacketType::{1})").format(self.name, self.ePacketType)
        if self.isReceived:
            data += " {\n\t*this"
            for var in self.variables:
                data += " >> " + var[1]
            data += ";\n}"
        else:
            for var in self.variables:
                data += ", {}({})".format(var[1], var[1][:-1])
            data += " {}"
        data += "\n\nRoseCommon::{0}::~{0}() {{}}".format(self.name)
        if not self.isReceived:
            data += "\n\nvoid RoseCommon::{}::pack(){{".format(self.name)
            data += "\n\t*this"
            for var in self.variables:
                data += " << " + var[1]
            data += ";\n}"
        return data

    def template(self):
        recv = ""
        if self.isReceived:
            recv = "recv"
        else:
            recv = "send"
        return ("template <>\nstruct find_" + recv + "_class<ePacketType::{}> {{ {} type; }};").format(self.ePacketType, self.name)

print("Welcome to the packet generator")
ePacketType = input("Please enter the ePacketType enum : ")
isReceived = "PAKCS_" in ePacketType
className = "Cli" if isReceived else "Srv"
className += ePacketType[6:].title().translate({ord('_') : None})

classRep = Class(className, ePacketType)

while True:
    value = menu()
    if value == 3:
        break
    elif value == 1:
        varType = input("Type: ")
        varName = input("Name: ")
        classRep.addType(varType, varName)
print(classRep.header())
print()
print(classRep.cpp())
print()
print(classRep.template())
