groundtruthConvert README
=========================

groundtruthConvert is to tool to convert between csv and different ground truth data
formats of the biorobotics lab. 

## Implemented features

* Print *.tdat as csv
* Convert *.tdat to csv 

## Building and usage

* 	Requires alls dependencies of biorobotics_core and pipeline
*	Requires boost program_options and opencv

* 	use:   
	cmake .  
	make  
	to build the library.

* 	use:  
	cmake cmake -DBUILD_CLI=1 .  
	make  
	to build the command line tool.

* 	For groundtruthConvertCLI, try   
	groundtruthConvertCLI --help  
	to see the help text for conversion details.

## Todo

* Conversion between gt formats
* csv -> tdat
