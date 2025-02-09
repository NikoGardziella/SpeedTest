#include <fstream>
#include <string>
#include <vector>


void ModifyConfigFile(std::string fileName, int sensorIndex, std::string newIP);

int main(int argc, char* argv[])
{
	if(argc < 3 || argc > 4)
	{
		printf("insert: filename, sensorindex(optional), newIP");
		return -1;
	}
	
	// filename should be first parameter
	std::string fileName = argv[1];

	int sensorIndex = -1; // -1 if the sensorIndex is not specified
	if (argc == 4)
	{
		// which sensor ip we are going to modify
		sensorIndex = std::stoi(argv[2]);
	}
	// newIp should always be the last parameter
	std::string newIP = argv[argc - 1];

	ModifyConfigFile(fileName, sensorIndex, newIP);
	return 0;
}

void ModifyConfigFile(std::string fileName, int sensorIndex, std::string newIP)
{
	std::ifstream myfile;
	myfile.open(fileName);
	if (!myfile)
	{
		printf("error opening the file");
		return;
	}

	std::vector<std::string> newOutput; // this will be written to the same file
	std::string line;
	int currentIndex = -1;

	while (std::getline(myfile, line)) // read line one by one
	{
		if (line.empty())
		{
			// ignore empty lines add to the new output without further processing
			newOutput.push_back(line);
			continue;
		}

		size_t startPos = line.find('[');
		size_t endPos = line.find(']');

		// If both '[' and ']' exist in same lane and [ is before ]
		if (startPos != std::string::npos && endPos != std::string::npos && startPos < endPos)
		{
			// get the index from the string
			std::string indexStr = line.substr(startPos + 1, endPos - startPos - 1);
			if(indexStr.empty() || !std::isdigit(indexStr[0]))
			{
				currentIndex = -1;
			}
			else
			{
				currentIndex = std::stoi(indexStr); 
			}
			
		}

		if (line.find("SocketIp=") != std::string::npos) // look for "SocketIp="
		{
			// Found Socketip from the line
			if (sensorIndex == -1 /* if there is no sensor index ( -1 ), write new IP to all items */ ||
				currentIndex == sensorIndex) // write new ip to the given sensorIndex
			{
				line = "\t\t SocketIp=" + newIP;
			}
		}
		newOutput.push_back(line);
	}

	std::ofstream outFile(fileName);
	for (const std::string& newLine : newOutput)
	{
		// write newOutput to the same file line by line
		outFile << newLine << '\n';
	}
	outFile.close();

}


