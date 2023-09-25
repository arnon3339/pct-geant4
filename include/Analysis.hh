#ifndef ANALYSIS_HH
#define ANALYSIS_HH 1

#include <string>


namespace PCT
{
class Analysis
{

protected:
    Analysis(const std::string& tree, const std::string& fpath): tree_(tree), fpath_(fpath)
    {
    }

    static Analysis* singleton_;

    std::string tree_;
    std::string fpath_;

public:

    Analysis(Analysis &other) = delete;

    void operator=(const Analysis &) = delete;


    static Analysis *GetInstance(const std::string&, const std::string&);

    void GenOutFile(const std::string&);
};
    
} // namespace PCT


#endif