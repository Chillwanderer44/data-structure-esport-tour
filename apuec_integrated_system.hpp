#ifndef APUEC_INTEGRATED_SYSTEM_HPP
#define APUEC_INTEGRATED_SYSTEM_HPP

// Include all individual task headers
#include "spectator_manager.hpp"
#include "MatchScheduler.hpp"
#include "RegistrationManager.hpp"
#include "RegistrationSystem.hpp"
#include "Statistic.hpp"
#include <iostream>
#include <string>

using namespace std;

// Integrated System Class
class APUECIntegratedSystem {
private:
    // Task managers
    SpectatorManager* spectatorSystem;      // Task 3: Spectator Management
    MatchScheduler* matchScheduler;         // Task 1: Match Scheduling
    RegistrationManager* teamRegistration;  // Task 2: Team Registration
    RegistrationSystem* playerRegistration; // Task 2: Player Registration
    
    // System state
    bool isRegistrationActive;
    bool isTournamentActive;
    bool isSpectatorSystemActive;
    
    // System statistics
    int totalRegisteredTeams;
    int totalRegisteredPlayers;
    int totalSpectators;
    
    // Helper functions
    void displaySystemHeader();
    void displaySystemStatus();
    void displayTaskCredits();
    void waitForUserInput();

public:
    // Constructor and Destructor
    APUECIntegratedSystem();
    ~APUECIntegratedSystem();
    
    // Main system interface
    void displayMainMenu();
    void runSystem();
    
    // Task-specific menu handlers
    void handleTeamRegistrationMenu();      // Task 2A: Team Registration
    void handlePlayerRegistrationMenu();    // Task 2B: Player Registration  
    void handleSpectatorManagementMenu();   // Task 3: Spectator Management
    void handleMatchSchedulingMenu();       // Task 1: Match Scheduling
    void handleStatisticsMenu();           // Task 4: Statistics & History
    
    // System management
    void initializeTournament();
    void startTournament();
    void endTournament();
    void generateSystemReport();
    
    // Integration features
    void synchronizeData();
    void validateSystemState();
    void exportAllData();
};

#endif