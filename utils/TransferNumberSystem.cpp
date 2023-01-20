#include "TransferNumberSystem.hpp"

static std::string mass_simvol = "0123456789abcdef";

std::string revers_str(std::string s) {
    std::string tmp;

    for (int i = s.size() - 1; i > -1; i--) {
        tmp += s[i];
    }
    return (tmp);
}

std::string transfer_number_system(int number, int sis_ch)
{
	std::string str = "";


	while (number > 0)
	{
		str += mass_simvol[number % sis_ch];
		number /= sis_ch;
	}

	return (revers_str(str));
}

int     pow(int number, int pow) {
    if (pow == 0) {
        return (1);
    }

    int result = number;
    
    while (--pow) {
        result *= number;
    }
    return (result);
}

int     transfer_number_system(std::string number) {
    int count_simvols = number.size();
    int revers_count_simvols = count_simvols - 1;
    int result = 0;

    while (--count_simvols > -1) {
        result += (mass_simvol.find(number[count_simvols]) * pow(16, revers_count_simvols - count_simvols));
    }
    return (result);
}
