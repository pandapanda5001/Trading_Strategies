double position = 0; // position status parameter, the default is 0 
bool onTick(string symbol){ // onTick function, all strategy logic are in this function
    auto ct = exchange.SetContractType(symbol); // set the contract type and trading variety
    if(ct == false){ // if the setting contract type and trading variety is not successful 
        return false; // return false
    }
    auto r = exchange.GetRecords(); // get the k-line array
    if(!r.Valid || r.size() < 10){ // if getting the k-line array or the number of k-line is less than 10
        return false; // return false
    }
    auto arr = TA.KDJ(r, 9, 3, 3); // calculate the KDJ indicator 
    auto k = arr[0][arr[0].size() - 2]; // get the K value of the previous K line 
    auto d = arr[1][arr[1].size() - 2]; // get the D value of the previous K line
    auto dPre = arr[1][arr[1].size() - 3]; // get the D value of the second last of the K line
    string action; // define a string variable action
    // if currently holding long position, and the previous K line's D value is less than the second last k line's D value, close all position
    // if currently holding short position, and the previous K line's D value is greater than the second last k line's D value, close all position
    if((d < dPre && position > 0) || (d > dPre && position <0)){
        action = "cover";
    }else if (k > d && position <= 0){ // if the previous K line's K value is greater than the previous K line's D value, and there are no long positions
        action = "buy"; // set the variable action to "buy"
    }else if (k < d && position >= 0){ // if the previous K line's K value is less than the previous K line's D value, and there are no short positions
        action = "sell"; // set the variable action to "sell"
    }
    if (action.size() > 0){ // if there are placing order instruction
        position = ext::Trade(action, symbol, 1); // calling the C++ trading class library, placing orders according the direction of variable "action". and also renew the position status. 
    }
    return true; // return true
    } 
}
void main(){ // program starts from here
    while (true){  // enter the loop
        if (exchange.IO("status") == 0) // if the connection with the exchange if not stable.
            sleep(1000); // pause for 1 second
            continue; // skip this loop, enter the next loop
        }
        if(!onTick("this_week")){ // if the connection with the exchange is stable, enter this if loop, start executing the onTick function
            sleep(1000); // pause for 1 second
        }
    }
}
