#pragma once
#include <string>
// Graph class for storing cities and routes
class Graph
{
private: 
	bool bCheckCities;
	const int numberofCities = 20;
	// Since citylist array and adjacency matrice(arrRoutes) are not dynamic. Existence of cities
	// are controlled using this array bCityExists
	bool bCityExist[20]; 
	int	arrRoutes[20][20];
	std::string arrAllCities[20] = { "Probia", "Machineland", "Las Datas", "San Antenna", "West Circuitta",
	"East Circuitta", "Logicheim", "Analograd", "Island of Semicon",
	"Biomedberg", "Magneta", "Waverium", "Telecommunicastan", "Prokontrolkka", "Signale",
	"North Kirchoff", "South Kirchoff", "Gulf of Archi", "New Oerstad", "Neurals Deep" };
	
public: 
		
	Graph();
	bool checkCityExists(int i) { return bCityExist[i]; } ; 
	void AddGCity(std::string sCity);
	void RemoveGCity(std::string sCity);
	int SearchGCity(std::string sCity);
	void HalveRouteFare(int iDeparted,int iArrival);
	void AddGRoute(std::string sDeparted, std::string sArrival,int iRouteFare);
	void RemoveGRoute(std::string sDeparted, std::string sArrival);
	int SearchGRoute(std::string sDeparted, std::string sArrival);
	void ListGRoutes();

};


	