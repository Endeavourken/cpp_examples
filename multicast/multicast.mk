##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=multicast
ConfigurationName      :=Debug
WorkspaceConfiguration := $(ConfigurationName)
WorkspacePath          :=/home/chunyun/Documents/Code/Cpp
ProjectPath            :=/home/chunyun/Documents/Code/Cpp/multicast
IntermediateDirectory  :=../build-$(ConfigurationName)/multicast
OutDir                 :=../build-$(ConfigurationName)/multicast
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=chunyun
Date                   :=04/11/20
CodeLitePath           :=/home/chunyun/.codelite
LinkerName             :=/usr/bin/g++-7
SharedObjectLinkerName :=/usr/bin/g++-7 -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=../build-$(ConfigurationName)/bin/$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :=$(IntermediateDirectory)/ObjectsList.txt
PCHCompileFlags        :=
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)/opt/include 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)pthread $(LibrarySwitch)gtest $(LibrarySwitch)gtest_main 
ArLibs                 :=  "pthread" "gtest" "gtest_main" 
LibPath                := $(LibraryPathSwitch). $(LibraryPathSwitch)/opt/lib 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/ar rcu
CXX      := /usr/bin/g++-7
CC       := /usr/bin/gcc-7
CXXFLAGS :=  -g -O0 -Wall -std=c++11 $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=../build-$(ConfigurationName)/multicast/ucp_sock.cc$(ObjectSuffix) ../build-$(ConfigurationName)/multicast/address_test.cc$(ObjectSuffix) ../build-$(ConfigurationName)/multicast/utility.cc$(ObjectSuffix) ../build-$(ConfigurationName)/multicast/socket.cc$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: MakeIntermediateDirs $(OutputFile)

$(OutputFile): ../build-$(ConfigurationName)/multicast/.d $(Objects) 
	@mkdir -p "../build-$(ConfigurationName)/multicast"
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

MakeIntermediateDirs:
	@mkdir -p "../build-$(ConfigurationName)/multicast"
	@mkdir -p ""../build-$(ConfigurationName)/bin""

../build-$(ConfigurationName)/multicast/.d:
	@mkdir -p "../build-$(ConfigurationName)/multicast"

PreBuild:


##
## Objects
##
../build-$(ConfigurationName)/multicast/ucp_sock.cc$(ObjectSuffix): ucp_sock.cc ../build-$(ConfigurationName)/multicast/ucp_sock.cc$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/chunyun/Documents/Code/Cpp/multicast/ucp_sock.cc" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ucp_sock.cc$(ObjectSuffix) $(IncludePath)
../build-$(ConfigurationName)/multicast/ucp_sock.cc$(DependSuffix): ucp_sock.cc
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT../build-$(ConfigurationName)/multicast/ucp_sock.cc$(ObjectSuffix) -MF../build-$(ConfigurationName)/multicast/ucp_sock.cc$(DependSuffix) -MM ucp_sock.cc

../build-$(ConfigurationName)/multicast/ucp_sock.cc$(PreprocessSuffix): ucp_sock.cc
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) ../build-$(ConfigurationName)/multicast/ucp_sock.cc$(PreprocessSuffix) ucp_sock.cc

../build-$(ConfigurationName)/multicast/address_test.cc$(ObjectSuffix): address_test.cc ../build-$(ConfigurationName)/multicast/address_test.cc$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/chunyun/Documents/Code/Cpp/multicast/address_test.cc" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/address_test.cc$(ObjectSuffix) $(IncludePath)
../build-$(ConfigurationName)/multicast/address_test.cc$(DependSuffix): address_test.cc
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT../build-$(ConfigurationName)/multicast/address_test.cc$(ObjectSuffix) -MF../build-$(ConfigurationName)/multicast/address_test.cc$(DependSuffix) -MM address_test.cc

../build-$(ConfigurationName)/multicast/address_test.cc$(PreprocessSuffix): address_test.cc
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) ../build-$(ConfigurationName)/multicast/address_test.cc$(PreprocessSuffix) address_test.cc

../build-$(ConfigurationName)/multicast/utility.cc$(ObjectSuffix): utility.cc ../build-$(ConfigurationName)/multicast/utility.cc$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/chunyun/Documents/Code/Cpp/multicast/utility.cc" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/utility.cc$(ObjectSuffix) $(IncludePath)
../build-$(ConfigurationName)/multicast/utility.cc$(DependSuffix): utility.cc
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT../build-$(ConfigurationName)/multicast/utility.cc$(ObjectSuffix) -MF../build-$(ConfigurationName)/multicast/utility.cc$(DependSuffix) -MM utility.cc

../build-$(ConfigurationName)/multicast/utility.cc$(PreprocessSuffix): utility.cc
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) ../build-$(ConfigurationName)/multicast/utility.cc$(PreprocessSuffix) utility.cc

../build-$(ConfigurationName)/multicast/socket.cc$(ObjectSuffix): socket.cc ../build-$(ConfigurationName)/multicast/socket.cc$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/chunyun/Documents/Code/Cpp/multicast/socket.cc" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/socket.cc$(ObjectSuffix) $(IncludePath)
../build-$(ConfigurationName)/multicast/socket.cc$(DependSuffix): socket.cc
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT../build-$(ConfigurationName)/multicast/socket.cc$(ObjectSuffix) -MF../build-$(ConfigurationName)/multicast/socket.cc$(DependSuffix) -MM socket.cc

../build-$(ConfigurationName)/multicast/socket.cc$(PreprocessSuffix): socket.cc
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) ../build-$(ConfigurationName)/multicast/socket.cc$(PreprocessSuffix) socket.cc


-include ../build-$(ConfigurationName)/multicast//*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r $(IntermediateDirectory)


