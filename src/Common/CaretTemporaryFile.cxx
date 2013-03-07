
/*LICENSE_START*/
/*
 * Copyright 2013 Washington University,
 * All rights reserved.
 *
 * Connectome DB and Connectome Workbench are part of the integrated Connectome 
 * Informatics Platform.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *    * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *    * Neither the names of Washington University nor the
 *      names of its contributors may be used to endorse or promote products
 *      derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR  
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
 * SUBSTITUTE GOODS OR  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
 * POSSIBILITY OF SUCH DAMAGE.
 */
/*LICENSE_END*/

#define __CARET_TEMPORARY_FILE_DECLARE__
#include "CaretTemporaryFile.h"
#undef __CARET_TEMPORARY_FILE_DECLARE__

#include <QTemporaryFile>

#include "CaretHttpManager.h"

using namespace caret;


    
/**
 * \class caret::CaretTemporaryFile 
 * \brief Reads and writes a temporary file.
 * \ingroup Common
 *
 * Reads and writes a temporary file.  When an instance of this class
 * goes out of scope, the temporary file will be deleted.  This class
 * is able to read a file that resides on an HTTP server (filename 
 * starts with "http://").
 *
 * QTemporaryFile is encapsulated by this class.
 */

/**
 * Constructor.
 */
CaretTemporaryFile::CaretTemporaryFile()
: DataFile()
{
    m_temporaryFile = NULL;
    initializeCaretTemporaryFile();
}

/**
 * Destructor.
 */
CaretTemporaryFile::~CaretTemporaryFile()
{
    delete m_temporaryFile;
}

/**
 * Initialize the temporary file..
 */
void
CaretTemporaryFile::initializeCaretTemporaryFile()
{
    if (m_temporaryFile != NULL) {
        delete m_temporaryFile;
    }
    m_temporaryFile = new QTemporaryFile();
    setFileName(m_temporaryFile->fileName());
}

/**
 * Clear the temporary file.
 * Destroys the encapsulated QTemporaryFile.
 */
void
CaretTemporaryFile::clear()
{
    DataFile::clear();
    initializeCaretTemporaryFile();
}

/**
 * Is the file empty (contains no data)?
 *
 * @return
 *    true if the file is empty, else false.
 */
bool
CaretTemporaryFile::isEmpty() const
{
    const bool fileEmpty = (m_temporaryFile->size() <= 0);
    return fileEmpty;
}

AString
CaretTemporaryFile::getFileName() const
{
    return m_temporaryFile->fileName();
}

AString
CaretTemporaryFile::getFileNameNoPath() const
{
    CaretTemporaryFile* ctf = const_cast<CaretTemporaryFile*>(this);
    ctf->setFileName(m_temporaryFile->fileName());
    return DataFile::getFileNameNoPath();
}

/**
 * This method does nothing as the temporary file's name
 * is generated by QTemporaryFile.
 *
 * @param filename
 *     Name for file.
 */
void
CaretTemporaryFile::setFileName(const AString& filename)
{
    /*
     * Needed for getFileNameNoPath() functionality.
     */
    DataFile::setFileName(filename);
}

/**
 * Read the file at the given path into the temporary file.
 *
 * @param filename
 *    Name of the data file.
 * @throws DataFileException
 *    If the file was not successfully read.
 */
void
CaretTemporaryFile::readFile(const AString& filename) throw (DataFileException)
{
    if (DataFile::isFileOnNetwork(filename)) {
        /*
         * Read file on network.
         * Sort of a kludge, read from the network as a string of bytes
         * and then write the bytes to a temporary file.  Lastly,
         * read the temporary file as a VolumeFile.
         */
        CaretHttpRequest request;
        request.m_method = CaretHttpManager::GET;
        request.m_url = filename;
        CaretHttpResponse response;
        CaretHttpManager::httpRequest(request,
                                      response);
        if (response.m_ok == false) {
            QString msg = ("HTTP error retrieving: "
                           + filename
                           + "\nHTTP Response Code="
                           + AString::number(response.m_responseCode));
            throw DataFileException(msg);
        }
        
        const int64_t numBytes = response.m_body.size();
        if (numBytes > 0) {
            if (m_temporaryFile->open()) {
                const int64_t numBytesWritten = m_temporaryFile->write(&response.m_body[0],
                                                               numBytes);
                if (numBytesWritten != numBytes) {
                    throw DataFileException("Error reading remote file "
                                            + filename
                                            + "  Tried to write "
                                            + QString::number(numBytes)
                                            + " bytes to temporary file but only wrote "
                                            + AString::number(numBytesWritten)
                                            + " bytes.");
                }
                
                m_temporaryFile->close();
            }
            else {
                throw DataFileException("Unable to open temporary file for writing its content: "
                                        + filename);
            }
        }
        else {
            throw DataFileException("Failed to read any data from file: "
                                    + filename);
        }
    }
    else {
        /*
         * Read local file.
         */
        QFile file(filename);
        checkFileReadability(filename);
        if (file.open(QFile::ReadOnly)) {
            QByteArray byteArray = file.readAll();
            file.close();
            
            const int numBytes = byteArray.length();
            if (numBytes > 0) {
                if (m_temporaryFile->open()) {
                    const int64_t numBytesWritten = m_temporaryFile->write(byteArray);
                    if (numBytesWritten != numBytes) {
                        throw DataFileException("Error reading file "
                                                + filename
                                                + "  Tried to write "
                                                + QString::number(numBytes)
                                                + " bytes to temporary file but only wrote "
                                                + AString::number(numBytesWritten)
                                                + " bytes.");
                    }
                    
                    m_temporaryFile->close();
                }
                else {
                    throw DataFileException("Unable to open temporary file for writing its content: "
                                            + m_temporaryFile->fileName());
                }
            }
            else {
                throw DataFileException("No data read from file, is it empty?: "
                                        + filename);
            }
        }
        else {
            throw DataFileException("Unable to open file for reading its content: "
                                    + filename);
        }
    }
}

/**
 * Write the contents of the temporary file to a local file with
 * the given name.
 *
 * @param filename
 *    Name of the local data file.
 * @throws DataFileException
 *    If the file was not successfully written.
 */
void
CaretTemporaryFile::writeFile(const AString& filename) throw (DataFileException)
{
    checkFileWritability(filename);
    
    if (isEmpty()) {
        throw DataFileException("No data (temporary file is empty) to write to file: "
                                + filename);
    }

    const QString tempFileName = m_temporaryFile->fileName();
    QFile fileIn(tempFileName);
    if (fileIn.open(QFile::ReadOnly)) {
        QByteArray byteArray = fileIn.readAll();
        fileIn.close();
        
        QFile fileOut(filename);
        if (fileOut.open(QFile::WriteOnly)) {
            fileOut.write(byteArray);
            fileOut.close();
        }
        else {
            fileOut.close();
            throw DataFileException("Unable to open file for writing its content: "
                                    + filename);
        }
    }
    else {
        fileIn.close();
        throw DataFileException("Unable to open temporary file for reading its content: "
                                + tempFileName);
    }
    
}
