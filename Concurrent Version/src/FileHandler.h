#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <QObject>

class FileHandler: QObject
{
    Q_DISABLE_COPY(FileHandler)

public:
    FileHandler(QObject * parent = nullptr);

public:
  /**
    * @brief Create a QFile based on the specified file directory, read its values and store them in the given matrix.
    * This file could be selected from the file browser or dropped in.
    * @param fileDirectory The file's path where the floating point values to store are located.
    * @param targetMatrix A matrix to store the file contents.
    */
    void processFile(const QString& filePath, std::vector< std::vector<double> > &targetMatrix) const;

private:
   /**
    * @brief Create a std::vector<double> with the specified row elements.
    * @param elements The floating-point values of the current row being readed.
    * @return A std::vector<double> contaning the specified row elements.
    */
     std::vector<double> getNewRow(const QStringList& elements) const;
};

#endif // FILEHANDLER_H
