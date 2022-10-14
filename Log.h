/**
 * ENTER DESCRIPTION HERE
 * @date 8/28/2022
 * @author Javier "Lopea" Sandoval (https://lopea.dev)
 * 
 */

#ifndef MYLOX_LOG_H
#define MYLOX_LOG_H

/**********************************************************************************************************************/
namespace Log
{
    inline void Error(int line, const std::string &message)
    {
        std::cerr << "ERROR: At Line [" << std::to_string(line) << "]: " << message << std::endl;
    }
}
/**********************************************************************************************************************/


#endif //MYLOX_LOG_H
