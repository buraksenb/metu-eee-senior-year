#pragma once
#include "Graph.h"
#include "FlightTreeNode.h"
#include <ctime>
#include <stack>
#include <string>

// Below here are prototypes of global functions which are implemented in GlobalFunctions.cpp


std::string flightCodeGenerator(Graph& allRoutes, std::string sDeparted,std::string sArrival);

void InitRouteMap(Graph& gAllRoutes);

void ListAllFlights(FlightTreeNode* flightNode);
void AddTFlight(FlightTreeNode*& flightNode, std::string flightcode);
std::string SearchTFlight(FlightTreeNode* flightNode, Graph& allRoutes, std::string sDeparted, std::string sArrival);

bool SearchTFlightwithFlightCode(FlightTreeNode* flightNode, std::string flightcode);
FlightTreeNode* RemoveTFlight(FlightTreeNode* flightNode, std::string flightcode);


void InitFlightPlan(FlightTreeNode*& flightNode);

std::string* AddMonthlyNewRoutes(Graph& allRoutes,std::string cityList[20] ,int N);
void AddMonthlyNewFlights(FlightTreeNode*& flightNode,std::string* newRoutes,int N);


std::stack<std::string> CancelUnpopularFlights(FlightTreeNode*& flightNode,int OccupancyRateThreshold);
void DiscardUnpopularRoutesandUpdatePrices(FlightTreeNode*& allFlights,Graph& allRoutes,std::string cityList[20], std::stack<std::string> unpopularFlights);


std::string* CheckandAddReturnRoutes(Graph& allRoutes,std::string cityList[20]);
void AddReturnFlights(FlightTreeNode*& flightNode, std::string* newReturnRoutes);

bool DisplayRouteOptions(FlightTreeNode*& flightNode, Graph& allRoutes, std::string cityList[20], std::string sDeparted, std::string sArrival);



