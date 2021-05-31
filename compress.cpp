#include <iostream>
#include <fstream>
#include <string>

class functions
{
private:
    std::string *data;
    int no_of_lines;

public:
    struct entry
    {
        unsigned int lineIndex;
        unsigned int freq;
        entry() : lineIndex(0), freq(0) {}
    };

    const int num_of_dic_elements = 16; //constant

    entry *freq_list; // no_of_lines
    entry *dictionary = new entry[num_of_dic_elements];

    void createFreqList();                            // output the freq list
    void updateFreqList(std::string line, int index); // sub function of createFreqList
    void sortFreqList();                              // sub function of createFreqList
    std::string decimalToBinary(int digits, int num);

    std::string directMatching(std::string line);
    std::string oneBitMisMatch(std::string line);
    std::string twoBitMisMatch(std::string line);
    std::string fourBitMisMatch(std::string line);
    std::string twoAnyBitMisMatch(std::string line);
    int *checkDiffereceForMasking(std::string line, std::string dictEntry);
    std::string bitmaskBased(std::string line);

    void compressFunction();

    functions(std::string *data, int no_of_lines)
    {
        this->data = data;
        this->no_of_lines = no_of_lines;
        this->freq_list = new entry[no_of_lines];
    }
};

std::string functions::decimalToBinary(int digits, int num)
{
    std::string decimal = "";
    int number = num;
    for (int i = 0; i < digits; i++)
    {
        decimal = std::to_string(number % 2) + decimal;
        number /= 2;
    }
    return decimal;
}

void functions::createFreqList()
{

    for (int i = 0; i < no_of_lines; i++)
    {
        std::string line = data[i];
        updateFreqList(line, i);
    }

    sortFreqList();
    for (int i = 0; i < num_of_dic_elements; i++)
    {
        dictionary[i] = freq_list[i];
        // std::cout << freq_list[i].lineIndex << "--" << freq_list[i].freq << std::endl;
        // std::cout << data[dictionary[i].lineIndex] << std::endl;
    }
    // int *a;
    // a = checkDiffereceForMasking("11111110000000100000000000000000", "11111110000000100000000000001000");
    // std::cout << *a << *(a + 1) << *(a + 2) << *(a + 3) << std::endl;
    // std::cout << data[dictionary[1].lineIndex]<<std::endl;
    // std::cout << bitmaskBased("11111110000000100000000000000000") << std::endl;
    // std::cout << bitmaskBased("10000110000000100000000000001000") << std::endl;
    // std::cout << fourBitMisMatch("11010000111100001111001000100001") << std::endl;
};

void functions::updateFreqList(std::string line, int index)
{
    bool updated = false;
    for (int i = 0; i < no_of_lines; i++)
    {
        if (line == data[freq_list[i].lineIndex])
        {

            freq_list[i].freq = freq_list[i].freq + 1;
            updated = true;
            break;
        }
    }
    if (!updated)
    {
        freq_list[index].lineIndex = index;
        freq_list[index].freq = 1;
    }
};

void functions::sortFreqList()
{
    for (int i = 1; i < no_of_lines; i++)
    {
        entry current = freq_list[i];
        int j = i - 1;
        while (j >= 0 && freq_list[j].freq < current.freq)
        {
            freq_list[j + 1] = freq_list[j];
            j--;
        }
        freq_list[j + 1] = current;
    }
}

std::string functions::directMatching(std::string line)
{
    for (int i = 0; i < num_of_dic_elements; i++)
    {
        if (line == data[dictionary[i].lineIndex])
        {
            return "111" + decimalToBinary(4, i);
        }
    }
    return "F";
}

std::string functions::oneBitMisMatch(std::string line)
{
    for (int i = 0; i < num_of_dic_elements; i++)
    {
        std::string element = data[dictionary[i].lineIndex];
        int mismatch_count = 0;
        int mismatch_index = 0;
        for (int j = 0; j < element.length(); j++)
        {
            if (element[j] != line[j])
            {
                mismatch_count++;
                mismatch_index = j;
            }
            if (mismatch_count > 1)
            {
                break;
            }
        }
        if (mismatch_count == 1)
        {
            return "011" + decimalToBinary(5, mismatch_index) + decimalToBinary(4, i);
        }
    }
    return "F";
}

std::string functions::twoBitMisMatch(std::string line)
{
    for (int i = 0; i < num_of_dic_elements; i++)
    {
        std::string element = data[dictionary[i].lineIndex];
        int mismatch_count = 0;
        int mismatch_index = 0;
        int j = 0;
        while (j < element.length())
        {
            if (element[j] != line[j])
            {
                if (element[j + 1] != line[j + 1])
                {
                    mismatch_count++;
                    mismatch_index = j;
                    j += 2;
                }
                else
                {
                    mismatch_count = 0;
                    break;
                }
            }
            else
            {
                j += 1;
            }

            if (mismatch_count > 1)
            {
                break;
            }
        }
        if (mismatch_count == 1)
        {
            return "100" + decimalToBinary(5, mismatch_index) + decimalToBinary(4, i);
        }
    }
    return "F";
}

std::string functions::fourBitMisMatch(std::string line)
{
    for (int i = 0; i < num_of_dic_elements; i++)
    {
        std::string element = data[dictionary[i].lineIndex];
        int mismatch_count = 0;
        int mismatch_index = 0;
        bool mismatch_found = false;
        for (int j = 0; j < element.length(); j++)
        {
            if (element[j] != line[j])
            {
                if (mismatch_count < 4)
                {
                    if(mismatch_count==0){
                        mismatch_index = j;
                    }
                    mismatch_found = true;
                    mismatch_count++;
                }
                else
                {
                    mismatch_count = 0;
                    mismatch_found = false;
                    break;
                }
            }
            else if (mismatch_found && mismatch_count < 4)
            {
                mismatch_count = 0;
                mismatch_found = false;
                break;
            }
        }
        if (mismatch_count == 4)
        {
            return "101" + decimalToBinary(5, mismatch_index) + decimalToBinary(4, i);
        }
    }
    return "F";
}

// std::string functions::twoAnyBitMisMatch(std::string line)
// {
//     for (int i = 0; i < num_of_dic_elements; i++)
//     {
//         std::string element = data[dictionary[i].lineIndex];
//         int mismatch_count = 0;
//         int location[2];
//         int next_location = 0;
//         int j = 0;
//         while (j < element.length())
//         {
//             if ((element.substr(j, j + 1) != line.substr(j, j + 1)) &&
//                 (element[j] != line[j]) &&
//                 (element.substr(j, j + 1).length() > 1))
//             {
//                 mismatch_count++;
//                 if (next_location < 2)
//                 {
//                     location[next_location] = j;
//                 }
//                 next_location++;
//                 j += 2;
//             }
//             else
//             {
//                 j++;
//             }
//             if (mismatch_count > 2)
//             {
//                 break;
//             }
//         }
//         if (mismatch_count == 2)
//         {
//             return "110" + decimalToBinary(5, location[0]) + decimalToBinary(5, location[1]) + decimalToBinary(4, i);
//         }
//     }
//     return "F";
// }

std::string functions::twoAnyBitMisMatch(std::string line)
{
    for (int i = 0; i < num_of_dic_elements; i++)
    {
        std::string element = data[dictionary[i].lineIndex];
        int mismatch_count = 0;
        int location[2];
        int next_location = 0;
        int j = 0;
        while (j < element.length())
        {
            if (element[j] != line[j])
            {
                mismatch_count++;
                if (next_location < 2)
                {
                    location[next_location] = j;
                }
                next_location++;
            }
            j += 1;

            if (mismatch_count > 2)
            {
                break;
            }
        }
        if (mismatch_count == 2)
        {
            return "110" + decimalToBinary(5, location[0]) + decimalToBinary(5, location[1]) + decimalToBinary(4, i);
        }
    }
    return "F";
}

int *functions::checkDiffereceForMasking(std::string line, std::string dictEntry)
{
    int startLoc = -1;
    static int result[4];

    for (int k = 0; k < 4; k++)
    {
        result[k] = -1;
    }

    int index = 0;
    for (int i = 0; i < line.length(); i++)
    {
        if (line[i] != dictEntry[i])
        {
            result[index] = i;
            index++;
            if (startLoc == -1 && i <= line.length() - 4)
            {
                startLoc = i;
            }
            else if (i >= startLoc + 4)
            {
                result[0] = -1;
                return result;
            }
        }
    }
    return result;
}

std::string functions::bitmaskBased(std::string line)
{
    for (int i = 0; i < num_of_dic_elements; i++)
    {
        int *check;
        check = checkDiffereceForMasking(line, data[dictionary[i].lineIndex]);
        if (*check != -1)
        {
            std::string mask = "1000";
            for (int j = 1; j < 4; j++)
            {
                if (*(check + j) != -1)
                {
                    mask[*(check + j) - *(check)] = '1';
                    // std::cout << *(check + j) - *(check)<<std::endl;
                }
            }
            return "010" + decimalToBinary(5, *check) + mask + decimalToBinary(4, i);
        }
    }
    return "F";
}

void functions::compressFunction()
{
    
    std::string compressedBinary;

    int runLenCount = 0;
    for (int i = 0; i < no_of_lines; i++)
    {
        std::string compressedBinaryLine = "F";
        int minimumLen = 32;
        std::string tempComp = "F";
        int tempLen;

        if (i > 0)
        {
            if (data[i] == data[i - 1])
            {
                if (runLenCount < 8)
                {
                    runLenCount += 1;
                    continue;
                }
                else
                {
                    //append the conpressed one
                    compressedBinary += "001" + decimalToBinary(3, runLenCount - 1);
                    std::cout << "001" + decimalToBinary(3, runLenCount - 1) << std::endl;
                    runLenCount = 0;
                }
            }
            else
            {
                if (runLenCount != 0)
                {
                    compressedBinary += "001" + decimalToBinary(3, runLenCount - 1);
                    std::cout << "001" + decimalToBinary(3, runLenCount - 1) << std::endl;
                    runLenCount = 0;
                }
            }
        }
        // std::cout << i << std::endl;
        for (int j = 0; j < 6; j++)
        {
            switch (j)
            {
            case 0:
                tempComp = bitmaskBased(data[i]);
                tempLen = tempComp.length();
                break;
            case 1:
                tempComp = oneBitMisMatch(data[i]);
                tempLen = tempComp.length();
                break;
            case 2:
                tempComp = twoBitMisMatch(data[i]);
                tempLen = tempComp.length();
                break;
            case 3:
                tempComp = fourBitMisMatch(data[i]);
                tempLen = tempComp.length();
                break;
            case 4:
                tempComp = twoAnyBitMisMatch(data[i]);
                tempLen = tempComp.length();
                break;
            case 5:
                tempComp = directMatching(data[i]);
                tempLen = tempComp.length();
                break;
            default:
                break;
            }
            if (tempComp != "F" && minimumLen > tempLen)
            {
                compressedBinaryLine = tempComp;
                minimumLen = tempLen;
            }
        }
        if (compressedBinaryLine != "F")
        {
            // do the appending part
            compressedBinary += compressedBinaryLine;
            std::cout << compressedBinaryLine << std::endl;
        }
        else
        {
            // append original binary
            compressedBinary += "000" + data[i];
            std::cout << "000" + data[i] << std::endl;
        }
    }
    std::cout << compressedBinary;
}

int main()
{
    int no_of_lines = 0;
    // defining the file
    std::string line;
    std::ifstream myfile("original.txt");

    // read number of lines
    while (getline(myfile, line))
    {
        no_of_lines++;
    }
    myfile.clear();
    myfile.seekg(0, std::ios::beg);

    // dynamic array to store
    std::string *dic = new std::string[no_of_lines];
    int count = 0;

    // storing line by line
    while (getline(myfile, dic[count]))
    {
        count++;
    }
    // std::cout << no_of_lines << std::endl;

    functions a(dic, no_of_lines);
    a.createFreqList();
    a.compressFunction();

    return 0;
}