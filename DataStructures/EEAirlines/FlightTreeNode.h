#pragma once
#include <iostream>
class FlightTreeNode
{
    private:
    FlightTreeNode* leftptr;
    FlightTreeNode* rightptr;
    std::string data;
    int iOccupancyRate;
    bool bCheckSuccessiveLowRate;
public:
    // Constructors for different inputs or no inputs at all
    FlightTreeNode(): leftptr(nullptr), rightptr(nullptr), data("0"), iOccupancyRate(0), bCheckSuccessiveLowRate(false)
    {}

    FlightTreeNode(FlightTreeNode* _left,FlightTreeNode* _right, const std::string _data)
    :leftptr(_left),rightptr(_right),data(_data), iOccupancyRate(0), bCheckSuccessiveLowRate(false)
    {}

    FlightTreeNode(const std::string _data): leftptr(nullptr), rightptr(nullptr), data(_data), iOccupancyRate(0), bCheckSuccessiveLowRate(false)
    {}
    // Getters for private fields
    std::string getData() const {return data;}
    FlightTreeNode* getLeftNode() const {return leftptr;}
    FlightTreeNode* getRightNode() const {return rightptr;}
    int getOccupancyRate() const { return iOccupancyRate; }
    bool getSuccessiveLowRate() const { return bCheckSuccessiveLowRate;}
	
    // Setters for private fields
    void setSuccessiveLowRate(bool newSuccessiveLowRate) { bCheckSuccessiveLowRate = newSuccessiveLowRate; }
    void setData(const std::string newData) {data = newData;}
    void setOccupancyRate(int newOccupancy) { iOccupancyRate = newOccupancy; }
    void setLeftNode(FlightTreeNode* newLeft) {leftptr = newLeft;}
    void setRightNode(FlightTreeNode* newRight) { rightptr = newRight;}

};

