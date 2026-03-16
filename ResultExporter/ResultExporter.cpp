#include "ResultExporter.h"
#include <fstream>

using json = nlohmann::json;

json ResultExporter::algebraicToJSON(const AlgebraicNumber &num, const std::string &label)
{
    json j;
    j["label"] = num.getLabel().empty() ? label : num.getLabel();
    j["degree"] = num.getDegree();
    j["d"] = num.getD().toString();
    j["norm"] = num.norm().toString();

    j["coefficients"] = json::array();
    for (int i = 0; i < num.getDegree(); ++i)
    {
        j["coefficients"].push_back(num.getCoefficient(i).toString());
    }
    return j;
}

std::string ResultExporter::toJSON(const AlgebraicNumber &num, const std::string &label)
{
    return algebraicToJSON(num, label).dump(4);
}

std::string ResultExporter::toJSONArray(const std::vector<const AlgebraicNumber *> &nums)
{
    json jArray = json::array();
    for (const auto *num : nums)
    {
        if (num)
            jArray.push_back(algebraicToJSON(*num, ""));
    }
    return jArray.dump(4);
}

std::string ResultExporter::toPerfJSON(const std::vector<PerfMetric> &metrics)
{
    json jArray = json::array();
    for (const auto &m : metrics)
    {
        jArray.push_back({{"method", m.methodName},
                          {"time_us", m.durationMicro},
                          {"d", m.dValue}});
    }
    return jArray.dump(4);
}

void ResultExporter::saveToFile(const std::string &jsonStr, const std::string &filename)
{
    std::ofstream outFile(filename);
    if (outFile.is_open())
    {
        outFile << jsonStr;
        outFile.close();
    }
}

void ResultExporter::addResult(const AlgebraicNumber &result)
{
    resultsJson.push_back(algebraicToJSON(result, ""));
}

void ResultExporter::save()
{
    std::ofstream outFile(filename);
    if (outFile.is_open())
    {
        outFile << resultsJson.dump(4);
        outFile.close();
    }
}

void ResultExporter::addRawResult(const std::string &label, int degree, const std::string &d, const std::vector<std::string> &coeffs, const std::string &norm)
{
    json j;
    j["label"] = label;
    j["degree"] = degree;
    j["d"] = d;
    j["norm"] = norm;
    j["coefficients"] = coeffs;
    resultsJson.push_back(j);
}