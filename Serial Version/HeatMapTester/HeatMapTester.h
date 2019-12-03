#ifndef HEATMAPTESTER_H
#define HEATMAPTESTER_H

#include <QCoreApplication>
#include <QFileInfo>

class HeatMapModel;
class QFileInfo;

class HeatMapTester : public QCoreApplication
{
    Q_OBJECT
    Q_DISABLE_COPY(HeatMapTester)

public:
    /**
     * @brief HeatMapTester constructor.
     * @param argc Number of parameters readed from the console.
     * @param argv The parameters readed from the console.
     */
    explicit HeatMapTester(int &argc, char **argv);

    /**
     * @brief Verify the correctness of the test cases contained in the specified directories.
     * @return Exit success code.
     */
    int run();

private:
    /**
     * @brief Print the correct input format to use HeatMapTester.
     * @return Exit failure code.
     */
    static int printHelp();

    /**
     * @brief Locate each input file on the current directory, run the heat simulation, and compare the
     * results obtained with its corresponding output file.
     * @param testDirectoryPath Current test directory path.
     * @return Exit sucess code.
     */
    int testDirectory(const QString& testDirectoryPath);

    /**
     * @brief Extract the test case number and its respective epsilon by splitting the file name.
     * @param fileInfo To obtain the file name to be splitted.
     * @return A QStringList containing the test case number and its respective epsilon.
     */
    QStringList getTestCaseInfo(const QFileInfo& fileInfo);

    /**
     * @brief Run the heat simulation with the specified input CSV file until the temperature stabilizes.
     * @param epsilon The maximum temperature difference between one generation and another
     * to determine if the heat simulation has stabilized.
     * @param inputCsvFilePath The path where the CSV input file is located.
     * @return Exit success code.
     */
    int runTestCase(const double& epsilon, const QString& inputCsvFilePath);

    /**
     * @brief Load the ideal temperature matrix from the corresponding CSV output file.
     * @param outputCsvFilename The path where the output CSV file is located.
     * @return Exit sucess code.
     */
    int loadOutput(const QString& outputCsvFilename);

    /**
     * @brief Determine the correctness of the final temperature matrix by comparing it with the ideal CSV output file.
     * The maximum difference in the precision of the temperatures is readed from the CSV output file.
     * @param outputCsvFilename The path of output CSV file where the precision difference is located.
     */
    void compareContents(const QString& outputCsvFilename);

    /**
     * @brief Reads from a CSV output file the desired precision difference to determine the correctness
     * of the final temperature matrix.
     * @param outputCsvFilename The path of CSV output file where the precision difference is located.
     * @return The precision difference.
     */
    double detectPrecisionDifference(const QString& outputCsvFilename);

private:
    std::vector< std::vector <double> > outputMatrix;
    HeatMapModel * heatMapModel = nullptr;
};


#endif // HEATMAPTESTER_H
