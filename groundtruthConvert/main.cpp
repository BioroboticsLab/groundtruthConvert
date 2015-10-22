
#include "converter.h"
#include <iostream>
#include <fstream>
#include <stdio.h>

#include "boost/program_options.hpp"


boost::program_options::variables_map getQcCLIoptions(int argc, char **argv){
	namespace po = boost::program_options;
	namespace po_style = boost::program_options::command_line_style;

	// Declare the supported options.
	po::options_description desc("Allowed options");
	desc.add_options()
							("help,h", 			"Produce help message")
							("echo", 			"Print file content as csv to std::out. Requires --in and --intype.")
							("convert", 		"Converts a file. Requires --in, --out, --intype, --outtype")
							//("unused",		po::value<int>(),"")
							("in",	 			po::value<std::string>(),"Input file.")
							("out", 			po::value<std::string>(),"Output file.")
							("intype", 			po::value<std::string>(),"Input file type.")
							("outtype",			po::value<std::string>(),"Output file type.")
							;

	po::variables_map vm;
	try{
		po::store(po::command_line_parser(argc, argv).options(desc)
				.style(	po_style::unix_style
						| po_style::case_insensitive
						| boost::program_options::command_line_style::allow_long_disguise
				).run(), vm);
		po::notify(vm);
	}catch( boost::program_options::unknown_option & e )
	{
		std::cerr << e.what() <<std::endl;
		std::cout << desc << std::endl;
		std::cout << "Supported file types: tdat, csv" << std::endl;
		exit(0);
	}catch( ... )
	{
		std::cerr <<"Unrecognized exception. Did you forget to set a value?\n" ;
		exit(0);
	}
	if (vm.count("help") || argc<1) {
		std::cout << desc << std::endl;
		return 1;
	}

	return vm;
}

bool filetype(std::string str, std::string type){
	//Is there a smarter way to do this?
	std::transform(str.begin(), str.end(),str.begin(), ::toupper);
	std::transform(type.begin(), type.end(),type.begin(), ::toupper);

	int s = type.size();
	if(str.size()<s) return false;

	if(str.substr(str.size()-s)==type) return true;
	return false;
}

bool cmpNoCase(std::string str, std::string str2){

	std::transform(str.begin(), str.end(),str.begin(), ::toupper);
	std::transform(str2.begin(), str2.end(),str2.begin(), ::toupper);
	return (str==str2);
}

int main(int argc, char **argv){

	gtConverter::gtWorker w;
	boost::program_options::variables_map vm = getQcCLIoptions(argc,argv);
	std::string infile,outfile,intype,outtype;

	//Set and verify all parameters.

	if(vm.count("echo") || vm.count("convert")){
		if (vm.count("in")){
			infile = vm["in"].as<std::string>();
			std::ifstream my_file(infile.c_str());
			if (!my_file.good())
			{
				std::cerr << "Error: Input file not readable." << std::endl;
				exit(1);
			}
		}else{
			std::cerr << "Error: Please specify an input file." << std::endl;
			exit(1);
		}

		if (vm.count("intype")){
			intype = vm["intype"].as<std::string>();
			if(! cmpNoCase(intype,"tdat") && ! cmpNoCase(intype,"csv") ){
				std::cerr << "Error: Input type not valid." << std::endl;
				exit(1);
			}
		}else if(filetype(infile,"csv")){
			std::cout << "Notice:Assuming filetype csv for input." << std::endl;
			intype = "csv";
		}else if(filetype(infile,"tdat")){
			std::cout << "Notice:Assuming filetype tdat for input." << std::endl;
			intype = "tdat";
		}

		if(intype==""){
			std::cerr << "Error: Please specify an input type." << std::endl;
			exit(1);
		}
	}

	if(vm.count("convert")){
		if (vm.count("out")){
			outfile = vm["out"].as<std::string>();
			std::ifstream my_file(outfile.c_str());
			if (my_file.good())
			{
				std::cout << "Warning: Overwriting old output file." << std::endl;
			}
		}else{
			std::cerr << "Error: Please specify an output file." << std::endl;
			exit(1);
		}


		if (vm.count("outtype")){
			outtype = vm["outtype"].as<std::string>();
			if(! cmpNoCase(outtype,"tdat") && ! cmpNoCase(outtype,"csv") ){
				std::cerr << "Error: Output type not valid." << std::endl;
				exit(1);
			}
		}else if(filetype(outfile,"csv")){
			std::cout << "Notice:Assuming filetype csv for output." << std::endl;
			outtype = "csv";
		}else if(filetype(outfile,"tdat")){
			std::cout << "Notice:Assuming filetype tdat for output." << std::endl;
			outtype = "tdat";
		}

		if(outtype==""){
			std::cerr << "Error: Please specify an output type." << std::endl;
			exit(1);
		}
	}

	//Run echo'ing of files
	if(vm.count("echo")){
		if(cmpNoCase(intype,"tdat") )
			w.printTDatFile(infile);
		else if(cmpNoCase(intype,"csv") ){
			//uhm, this is basically a cat command, but well... :)
			std::copy(infile.begin(), infile.end(), std::ostream_iterator<char *>(std::cout, ""));
		}
	}

	//Run file conversion
	else if(vm.count("convert")){
		if(cmpNoCase(intype,"tdat") && cmpNoCase(outtype,"csv")){
			std::string csv = w.TDatToCSV(infile);
			std::ofstream ostream(outfile.c_str(), std::ofstream::binary);
			ostream.write (csv.c_str(),csv.size());
			ostream.close();
		}else
		{
			std::cout << "Sorry, the specified conversion from "<<intype << " to " << outtype << " is not implemented yet." << std::endl;
			exit(1);
		}
	}

	//Nothing to run?
	else{

		std::cerr << "Error: No action specified. Exiting..." << std::endl;
		exit(1);
	}
	return 0;
}








