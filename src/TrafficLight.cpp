#include <iostream>
#include <random>
#include "TrafficLight.h"

/* Implementation of class "MessageQueue" */


template <typename T>
T MessageQueue<T>::receive()
{
    // FP.5a : The method receive should use std::unique_lock<std::mutex> and 
    //_condition.wait() 
    // to wait for and receive new messages and pull them from the queue using move semantics. 
    // The received object should then be returned by the receive function.
    
    // Lock _queue for this thread 
    // use unique lock here for condition_variable wait function
    std::unique_lock<std::mutex> uLock(_mutex);
    // start a wait condition for this thread
    _condition.wait(uLock, [this]{return !_queue.empty(); });
    //  retrieve last element from queue
    T msg = std::move(_queue.back());
    // remove last element from queue
    _queue.pop_back();
    // return received message
    return msg;
    // lock is released out of scope
}

template <typename T>
void MessageQueue<T>::send(T &&msg)
{
    // FP.4a : The method send should use the mechanisms std::lock_guard<std::mutex> 
    // as well as _condition.notify_one() to 
    // add a new message to the queue and afterwards send a notification.
    // lock guard 
    std::lock_guard<std::mutex> uLock(_mutex);
    // push this message to queue, use std::move since msg is rvalue
    _queue.push_back(std::move(msg));
    // notify _condition_variable to release wait
    _condition.notify_one();
}


/* Implementation of class "TrafficLight" */


TrafficLight::TrafficLight()
{
    _currentPhase = TrafficLightPhase::red;
}

void TrafficLight::waitForGreen()
{
    // FP.5b : add the implementation of the method waitForGreen, in which an infinite while-loop 
    // runs and repeatedly calls the receive function on the message queue. 
    // Once it receives TrafficLightPhase::green, the method returns.
}

TrafficLightPhase TrafficLight::getCurrentPhase()
{
    return _currentPhase;
}

void TrafficLight::simulate()
{
    // FP.2b : Finally, the private method „cycleThroughPhases“ should be started in a 
    // thread when the public method „simulate“ is called. 
    // To do this, use the thread queue in the base class.
    threads.emplace_back(std::thread(&TrafficLight::cycleThroughPhases, this));  
}

// virtual function which is executed in a thread
void TrafficLight::cycleThroughPhases()
{
    // FP.2a : Implement the function with an infinite loop that measures the time between two loop cycles 
    // and toggles the current phase of the traffic light between red and green and sends an update method 
    // to the message queue using move semantics. The cycle duration should be a random value between 4 and 6 seconds. 
    // Also, the while-loop should use std::this_thread::sleep_for to wait 1ms between two cycles.
    _currentPhase = TrafficLightPhase::red;
    int lBnd = 4000;
    int hBnd = 6000;
    int waitForPhase = (rand() % (hBnd - lBnd + 1)) + lBnd;
    int counter = 0; 
    while(true){
        std::this_thread::sleep_for(std::chrono::milliseconds(1)); // wait for 1 ms
        counter++;
        if(counter == waitForPhase){
            _currentPhase = (_currentPhase == TrafficLightPhase::red) ? TrafficLightPhase::green : TrafficLightPhase::red;
            counter = 0;
            // TODO: 
            // send update method to message queue
            
        }
    } 
}
