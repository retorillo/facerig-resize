Proj=build.xml
Release=/p:Configuration=Release
Debug=/p:Configuration=Debug
x64=/p:Platform=x64
x86=/p:Platform=x86

build:
	msbuild ${Proj} ${Release} ${x64} /t:Build
	msbuild ${Proj} ${Release} ${x86} /t:Build
debug:
	msbuild ${Proj} ${Debug} ${x64} /t:Build
	msbuild ${Proj} ${Debug} ${x86} /t:Build
clean:
	msbuild ${Proj} ${Release} ${x64} /t:Clean
	msbuild ${Proj} ${Debug} ${x64} /t:Clean
	msbuild ${Proj} ${Release} ${x86} /t:Clean
	msbuild ${Proj} ${Debug} ${x86} /t:Clean
