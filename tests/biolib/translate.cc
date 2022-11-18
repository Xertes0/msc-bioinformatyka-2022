#include <catch2/catch_test_macros.hpp>

#include <bio/translate.hh>

TEST_CASE("translate", "[biolib]")
{
    REQUIRE(bio::translate(std::string{"GACTTTATATTTAGTGTTTGGAGCTTGATCTGCCATAGTAGGTACTGCTATAAGTGTGCTAATTCGGGTTGAATTAGGTCAACCTGGGAGATTATTAGGGGATGATCAATTATACAATGTAATTGTAACAGGACATGCTTTTGTAATGATTTTTTTTATAGTTATACCAATTTTAATTGGAGGTTTTGGAAATTGGTTAGTTCCTTTGATATTAGGAGCTCCCGATATAGCTTTTCCTCGGATAAATAATTTGAGATTTTGATTATTACCTCCTTCTTTATTATTATTATTTATTTCTTCTATAGAAGAAATAGGGGTTGGGGCAGGTTGGACTATTTACCCTCCTTTATCTTCTTTAGAAGGGCATAGAGGGAGCTCTGTAGATTTTGCTATTTTCTCTTTACATTTAGCAGGTGCTTCATCTATTATAGGGGCTATTAATTTTATTTCTACTATTTTTAATATACGTATATATGGTATAAGATTAGAAAAGGTTAGATTGTTTGTTTGGTCGGTTTTAATTACAGCTGTATTGTTATTATTATCATTACCAGTATTAGCAGGGGCTATTACTATATTATTAACCGATCGAAATTTTAATACTTCATTTTTTGATCCTGCTGGGGGAGGTGATCCTATTTTATTTCAACATTTATTT"}) == "DFIFSVWSLICHSRYCYKCANSG-IRSTWEIIRG-SIIQCNCNRTCFCNDFFYSYTNFNWRFWKLVSSFDIRSSRYSFSSDK-FEILIITSFFIIIIYFFYRRNRGWGRLDYLPSFIFFRRA-RELCRFCYFLFTFSRCFIYYRGY-FYFYYF-YTYIWYKIRKG-IVCLVGFNYSCIVIIIITSISRGYYYIINRSKF-YFIF-SCWGR-SYFISTFI");
}