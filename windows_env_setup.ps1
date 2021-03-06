$mysql_version = "5.7";
$mysql_connector_version = "6.1.6";
$config = "Release"
$unit_tests = "OFF"

Import-Module .\ci\extrafunctions.psm1

function Generate-MySQLpp-Files
{
	[CmdletBinding()]
    Param(
        [parameter(
            Mandatory=$false,
            ValueFromPipeline=$true
        )]
        [String]$relativePath=".\"
    )
    $current_path = Get-AbsolutePath;
    $mysqlpp_root = Get-AbsolutePath($relativePath);
    Write-Host "$mysqlpp_root"
	
    cd $mysqlpp_root\lib
	perl querydef.pl
	perl ssqls.pl
    cd $current_path
}

function Install-MySQL-Connector-C
{
	$current_path = Get-AbsolutePath;

	Write-Host "Downloading Mysql Connector-C v$global:mysql_connector_version x86 to the $current_path"
	Start-FileDownload "http://dev.mysql.com/get/Downloads/Connector-C/mysql-connector-c-$global:mysql_connector_version-win32.msi" -FileName "$current_path/mysql-connector-c-$global:mysql_connector_version-winx32.msi"

    $installer = 'msiexec /i mysql-connector-c-' + $global:mysql_connector_version + '-winx32.msi /quiet /qn /norestart /log mysql-connector-c-' + $global:mysql_connector_version + '.log 2>&1';
	
	cmd /c $installer
}

function Update-Submodules
{
	cmd /c 'git submodule update --init --recursive 2>&1'
}

function GenerateProject {
  $current_path = Get-AbsolutePath;

  cmd /c 'mkdir build 2>&1'
  cd build

  $global:debug_build = BinaryQuestion("Is this a debug build?");
  if($global:debug_build) {
    $global:config = "Debug"
  } else {
    $global:config = "Release"
  }

  $cmake_command = 'cmake -DBUILD_TYPE=' + $global:config + ' -DBUILD_MYSQLPP=ON -DBUILD_PROTOBUF=ON -Dprotobuf_BUILD_TESTS=OFF -DENABLE_TESTING=' + $global:unit_tests + ' ..  2>&1'
  Write-Host "Running '$cmake_command'"
  cmd /c $cmake_command

  Write-Host "-------------------------------"
  Write-Host "Done generating project"
  Write-Host "-------------------------------"

  cd $current_path
}

function BuildProject {
  $current_path = Get-AbsolutePath;

  cd build
  $cmake_command = 'cmake --build . --config ' + $global:config + ' 2>&1'

  Write-Host "Running '$cmake_command'"
  cmd /c $cmake_command

  cd $current_path
}


$project_root = Get-AbsolutePath;

Write-Host "Checking out trunk branch"
cmd /c 'git checkout trunk 2>&1'

$build_unit_tests = BinaryQuestion("Build Unit Tests");
if($build_unit_tests) {
    $global:unit_tests = "ON";
}

$update_submodules = BinaryQuestion("Update submodules (needed for compile)");
if($update_submodules) {
    Update-Submodules
}

$install_mysql_connector = BinaryQuestion("Install MySQL Connector C");
if($install_mysql_connector) {
    Install-MySQL-Connector-C
}

$generate_mysqlpp = BinaryQuestion("Create the mysqlpp files");
if($generate_mysqlpp) {
    Generate-MySQLpp-Files("$project_root\tools\mysqlpp")
}

Write-Host "Generating the project now..."
GenerateProject

$build_project = BinaryQuestion("Build the project now?");
if($build_project) {
    BuildProject
}
