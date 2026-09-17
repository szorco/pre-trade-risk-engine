#pragma once
#include <cstdint>
#include <string>

#include <chrono> //represent time as number

/*
Order: Synthetic orders 
    - orderId: {integer} id to track order
        --> Store 64 bits of orders: There may be more than 4.2 billion orders
        -USAGE: increment count and store it as id number

    - ticker: {string} Financial instrument
        -USAGE: "IBM"

    - orderPrice: {integer?} price that we want to fulfill order at
    
    - side: {enumeration?} BUY or SELL
        -USAGE: Side::BUY, Side::SELL
    
    - quantity:  Number of shares (whole number)

    
    - timeStamp: time stamp of order being initially processed
        - down to nanosecond,, 

*/
enum class Side {BUY , SELL};

struct Order{
    std::uint64_t orderId;

    std::string ticker;  //TODO: Change to optimize
    
    std::uint32_t orderPrice;  //last 2 digits == decimal values
    Side orderSide; 
    std::uint32_t quantity;

    std::chrono::system_clock::time_point timestampInitial; //2026-09-16 19:33:51.123456789
};