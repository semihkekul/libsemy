#include <iostream>
#include <string>
char alphabet[] = {
	'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
};
std::string titles[] = {" "," "," "," "," "," "," ","SNA","SNA","SNB","SNB","ANB","ANB","ANPog","ANPog","CoA","CoA","NSL/PD","NSL/PD","CoGn","CoGn","Body length","Body length","Ramus height","Ramus height","RH-","RH-","PogNB","PogNB","SNPog","SNPog","Wits","Wits","Convexity angle","Convexity angle","Saddle angle","Saddle angle","Articular angle","Articular angle","Gonial angle","Gonial angle","Y-axis/NSL","Y-axis/NSL","PD/MD","PD/MD","SN-GoGn","SN-GoGn","AFH","AFH","PFH","PFH","LFH","LFH","LFH:AFH","LFH:AFH","ILs/NSL","ILs/NSL","ILs/NL","ILs/NL","ILs/NA","ILs/NA","Is-NA","Is-NA","ILi/NB","ILi/NB","Ii-NB","Ii-NB","Ii-ML","Ii-ML","InterIncisal","InterIncisal","Overbite","Overbite","OL-p angle","OL-p angle","Overjet","Overjet","Molar relation","Molar relation","Max base","Max base","Mand base","Mand base","Condyle","Condyle","Mand length","Mand length","Max inc","Max inc","Mand inc","Mand inc","Max molar","Max molar","Mand molar","Mand molar","Max inc-base","Max inc-base","Mand inc-base","Mand inc-base","Max mol-base","Max mol-base","Mand mol-base","Mand mol-base","A-NP","A-NP","Pog-NP","Pog-NP","Nasolabial","Nasolabial","Profile angle","Profile angle","Chin thickness","Chin thickness","Ls-EL","Ls-EL","Li-EL","Li-EL","Ls-SL","Ls-SL","Li-SL","Li-SL","IntL gap","IntL gap","Me Thick","Me Thick","MentoS","MentoS","IMPA","IMPA","ÜST DDK","ÜST DDK","ALT DDK","ALT DDK","Pgs","Pgs","NASION","NASION","RHINION","RHINION","SUBNSL","SUBNSL","LABSUB","LABSUB","STOMI-","STOMI-","STOMI-","STOMI-","LABINF","LABIFN","LABMENT","LABMENT","GNATH","GNATH","GLAB","GLAB"};
	

int main()
{
	int titleIndex = 0;
	for(int j = 1 ; j < 26; ++j)
	{
		
		if(j % 2 != 0)
		{
			std::cout << titles[titleIndex++];
			titleIndex++;
			std::cout << std::endl;
		}
		std::cout << alphabet[j]<<" ";
		
	}
	for(int i = 0; i < 26; ++i)
		for(int j = 0 ; j < 26; ++j)
		{
			if(j % 2 != 0)
			{
				std::cout << titles[titleIndex++];
				titleIndex++;
				std::cout << std::endl;
			}
			std::cout << alphabet[i] << alphabet[j]<<" ";
		}
}