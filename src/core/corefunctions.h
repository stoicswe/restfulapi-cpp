#include <algorithm>
#include <string>

/**
 * Given the passed command arguments from the command line, parse the arguments for a known
 * argument and its value (unless that argument is simply a flag). If the argument is an argument
 * and a value, pass the value for the argument passed, otherwise if the flag is present then pass the
 * flag itself. If the desired flag or argument is not found, pass a nullptr;
 *
 * @param begin
 * @param end
 * @param option
 * @param isFlag
 * @return
 */
static char* getCmdOption(char** begin, char** end, const std::string& option, bool isFlag)
{
    char** itr = std::find(begin, end, option);
    if (isFlag || (itr != end && ++itr != end))
    {
        return *itr;
    }
    return nullptr;
}

/**
 * Check if a flag or argument exists in the command line stream.
 *
 * @param begin
 * @param end
 * @param option
 * @return
 */
static bool cmdOptionExists(char** begin, char** end, const std::string& option)
{
    return std::find(begin, end, option) != end;
}
