#include "Graph.h"

#include <iostream>

Graph::Graph()
{
	bCheckCities = true;
	for (int i= 0 ; i < numberofCities ; i++)
	{
		bCityExist[i] = false;
	}
	for (int i = 0; i < numberofCities; i++)
	{
		for (int j = 0; j < numberofCities; j++)
		{
			arrRoutes[i][j]= -1;
		}
	}
}
// If a flight's occupancy is lower than the threshold for first month, route fare will be halved
void Graph::HalveRouteFare(int iDeparted,int iArrival)
{
	if (arrRoutes[iDeparted][iArrival] != 0)
	{
	 arrRoutes[iDeparted][iArrival] = (arrRoutes[iDeparted][iArrival]/2) ;
	}
}


void Graph::AddGCity(std::string sCity)
{
	int iIndex = -1;
	for (int i = 0; i < numberofCities; i++)
	{
		// First find the index of the city in city list array
		if (sCity == arrAllCities[i])
		{
			iIndex = i;
			break;
		}
	}
	// Check if that city already exists
	if(bCityExist[iIndex] == true)
	{
		std::cerr << sCity  <<" was already added!\n";
	}
	else
	{
		bCityExist[iIndex] = true;
	}
}

void Graph::RemoveGCity(std::string sCity)
{
	{
		int iIndex = -1;
		for (int i = 0; i < numberofCities; i++)
		{
			if (sCity == arrAllCities[i])
			{
				iIndex = i;
				break;
			}
		}
		if (bCityExist[iIndex] == false)
		{
			std::cerr << sCity << " cannot be removed since it does not exists!\n";
		}
		else
		{
			// When the city is removed, remove all routes related to that city
			for(int i=0; i<numberofCities ; i++)
			{
				arrRoutes[i][iIndex] = 0 ;
				arrRoutes[iIndex][i] = 0 ;
			}
			bCityExist[iIndex] = false;
		}
	}
}

int Graph::SearchGCity(std::string sCity)
{
	{
		{
			int iIndex = -1 ;
			for (int i = 0; i < numberofCities; i++)
			{
				if (sCity == arrAllCities[i])
				{
					iIndex = i;
					break;
				}
			}
			if(iIndex != -1)
			{

				if (bCityExist[iIndex] == false)
				{
					// city is not yet added to the route map
					return -1;
				}
				else
				{
					// city is in the routemap, thus return its index
					return iIndex;
				}
			}
			else
			{
				std::cerr << "City name is not defined in the given list\n";
				// Return -1 such that we can know if function returns -1 there is something wrong.
				return iIndex;
			}


		}
	}
}

void Graph::AddGRoute(std::string sDeparted, std::string sArrival,int iRouteFare)
{
	int iDeparted = SearchGCity(sDeparted);
	int iArrival = SearchGCity(sArrival);
	// Check both departed and arrival cities to exist and check that they are different
	if(bCityExist[iDeparted] && bCityExist[iArrival] && (iArrival != iDeparted))
	{
		arrRoutes[iDeparted][iArrival] = iRouteFare ;
	}
	else
	{
		std::cerr << "Either " << sDeparted << " or " << sArrival << " or both of them does not exists!\n" ;
	}

}

void Graph::RemoveGRoute(std::string sDeparted, std::string sArrival)
{
	int iSource = SearchGCity(sDeparted);
	int iDestination = SearchGCity(sArrival);
	if(bCityExist[iSource] && bCityExist[iDestination])
	{
		arrRoutes[iSource][iDestination] = 0 ;
	}
	else
	{
		std::cerr << "Either " << sDeparted << " or " << sArrival << " or both of them does not exists!\n" ;
	}
}

int Graph::SearchGRoute(std::string sDeparted, std::string sArrival)
{
	int iSource = SearchGCity(sDeparted);
	int iDestination = SearchGCity(sArrival);
	if(iSource == -1 ||iDestination == -1)
	{
		return -1;
	}
	if(bCityExist[iSource] && bCityExist[iDestination])
	{
		// return route fare if both cities exits, if there is no route returned value is 0
		return arrRoutes[iSource][iDestination];
	}
	else
	{
		return -1;
	}
}

void Graph::ListGRoutes()
{
	std::string sDeparted, sArrival;
	int iRouteFareofCity;
	std::cout << "Routes: \n";
	for (int iDeparted= 0  ; iDeparted < numberofCities ; iDeparted++  )
	{
		// If city does not exists dont iterate through that row
		if(bCityExist[iDeparted])
		{

			for(int iArrival= 0 ; iArrival< numberofCities ; iArrival++)
			{
				if(bCityExist[iArrival])
				{
					iRouteFareofCity = arrRoutes[iDeparted][iArrival] ;
					// If route fare is not -1 nor 0, then route exists.
					if(iRouteFareofCity != -1 && iRouteFareofCity != 0)
					{
						sDeparted = arrAllCities[iDeparted];
						sArrival = arrAllCities[iArrival];
						std::cout << "(" << sDeparted << "---> " << sArrival << ", " << iRouteFareofCity << " )\n" ;
					}

				}
			}

		}
	}
	std::cout << "\n";
}
