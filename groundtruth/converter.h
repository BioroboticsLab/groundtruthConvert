/*
 * converter.h
 *
 *  Created on: Oct 21, 2015
 *      Author: hauke
 */

#ifndef CONVERTER_H_
#define CONVERTER_H_

#include <string.h>
#include <stdio.h>
#include <iostream>
#include <vector>

namespace gtConverter {

/*! \class gtWorker gtWorker.h “gtWorker.h”
 * \brief Convert between csv and biorobtics ground truth formats
 *
 * Currently supports:
 * tdat -> csv
 * tdat -> csv -> std out
 *
 * Echo a tdat file in csv format:
 * \code
 * gtConverter::gtWorker w;
 * w.printTDatFile("myfile.tdat");
 * \endcode
 *
 */
class  gtWorker {

  private:

  public:

    /*! \brief Prints a tdat file as csv using std::cout
     *
     * \param file Path to tdat file
     * \return Nothing
     */
    void printTDatFile(std::string file);

    /*! \brief Returns a tdat file in csv format
     *
     * \param file Path to tdat file
     * \return Content in csv format (string)
     */
    std::string TDatToCSV(std::string file, bool appendGridpoints);
};
}



#endif /* CONVERTER_H_ */
