#ifndef RESULTEXPORTER_H
#define RESULTEXPORTER_H
#include <string>
#include <vector>
#include <chrono>

#include "../AlgebraicNumber/AlgebraicNumber.h"
#include "../Utilities/nlohmann/json.hpp"

struct PerfMetric
{
    std::string methodName;
    long long durationMicro;
    std::string dValue;
};

class ResultExporter
{
public:
    ResultExporter(const std::string &filename) : filename(filename)
    {

        resultsJson = nlohmann::json::array();
    }
    static std::string toJSON(const AlgebraicNumber &num, const std::string &label);
    static void saveToFile(const std::string &json, const std::string &filename);
    static std::string toJSONArray(const std::vector<const AlgebraicNumber *> &nums);
    static std::string toPerfJSON(const std::vector<PerfMetric> &metrics);
    void addRawResult(const std::string &label, int degree, const std::string &d, const std::vector<std::string> &coeffs, const std::string &norm);
    void addResult(const AlgebraicNumber &result);
    void save();

private:
    std::string filename;
    nlohmann::json resultsJson;
    static nlohmann::json algebraicToJSON(const AlgebraicNumber &num, const std::string &label);
};

#endif