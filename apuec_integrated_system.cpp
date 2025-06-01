#include "apuec_integrated_system.hpp"
#include <iomanip>
#include <fstream>

// Constructor
APUECIntegratedSystem::APUECIntegratedSystem() {
    // Initialize all task systems
    spectatorSystem = new SpectatorManager(20, 30, 100);  // VIP, Influencer, General seats
    matchScheduler = new MatchScheduler();
    teamRegistration = new RegistrationManager();
    playerRegistration = new RegistrationSystem();
    
    // Initialize system state
    isRegistrationActive = true;
    isTournamentActive = false;
    isSpectatorSystemActive = true;
    
    totalRegisteredTeams = 0;
    totalRegisteredPlayers = 0;
    totalSpectators = 0;
    
    cout << "APUEC Tournament Management System Initialized Successfully!\n";
}

// Destructor
APUECIntegratedSystem::~APUECIntegratedSystem() {
    delete spectatorSystem;
    delete matchScheduler;
    delete teamRegistration;
    delete playerRegistration;
}

void APUECIntegratedSystem::displaySystemHeader() {
    cout << "\n" << string(80, '=') << "\n";
    cout << "    ASIA PACIFIC UNIVERSITY ESPORTS CHAMPIONSHIP (APUEC)\n";
    cout << "              INTEGRATED TOURNAMENT MANAGEMENT SYSTEM\n";
    cout << string(80, '=') << "\n";
}

void APUECIntegratedSystem::displaySystemStatus() {
    cout << "\n--- SYSTEM STATUS ---\n";
    cout << "Registration: " << (isRegistrationActive ? "ACTIVE" : "CLOSED") << "\n";
    cout << "Tournament: " << (isTournamentActive ? "IN PROGRESS" : "NOT STARTED") << "\n";
    cout << "Spectator System: " << (isSpectatorSystemActive ? "ACTIVE" : "INACTIVE") << "\n";
    cout << string(50, '-') << "\n";
}

void APUECIntegratedSystem::displayTaskCredits() {
    cout << "\n--- DEVELOPMENT TEAM ---\n";
    cout << "Task 1 (Match Scheduling): [Daniel - Stack, Queue, Priority Queue, Circular Queue]\n";
    cout << "Task 2 (Registration): [Rin - Queue, Stack, Circular Queue, Priority Queue]\n";
    cout << "Task 3 (Spectator Management): [Amir - Priority Queue Implementation]\n";
    cout << "Task 4 (Statistics & History): [Daniel - Stack for Match History]\n";
    cout << string(70, '-') << "\n";
}

void APUECIntegratedSystem::displayMainMenu() {
    displaySystemHeader();
    displaySystemStatus();
    
    cout << "\n=== MAIN MENU ===\n";
    cout << "1. Team Registration Management (Task 2A)\n";
    cout << "2. Player Registration Management (Task 2B)\n";
    cout << "3. Spectator Management System (Task 3)\n";
    cout << "4. Match Scheduling & Tournament (Task 1)\n";
    cout << "5. Statistics & Match History (Task 4)\n";
    cout << "6. Tournament Control Center\n";
    cout << "7. Generate System Report\n";
    cout << "8. View Development Credits\n";
    cout << "9. Exit System\n";
    cout << string(50, '-') << "\n";
    cout << "Select option (1-9): ";
}

void APUECIntegratedSystem::handleTeamRegistrationMenu() {
    int choice;
    
    do {
        cout << "\n=== TEAM REGISTRATION MANAGEMENT (Task 2A) ===\n";
        cout << "Data Structure Used: Queue, Stack, Circular Queue, Priority Queue\n";
        cout << string(60, '-') << "\n";
        cout << "1. Register New Team\n";
        cout << "2. Withdraw Team\n";
        cout << "3. Replace Team\n";
        cout << "4. End Team Registration\n";
        cout << "5. View Registration Status\n";
        cout << "6. Back to Main Menu\n";
        cout << "Choice: ";
        cin >> choice;
        cin.ignore();
        
        switch (choice) {
            case 1:
                teamRegistration->registerTeam();
                totalRegisteredTeams++;
                break;
            case 2: {
                string teamName;
                cout << "Enter team name to withdraw: ";
                getline(cin, teamName);
                teamRegistration->withdrawTeam(teamName.c_str());
                totalRegisteredTeams--;
                break;
            }
            case 3: {
                string oldName, newName;
                cout << "Enter team name to replace: ";
                getline(cin, oldName);
                cout << "Enter new team name: ";
                getline(cin, newName);
                teamRegistration->replaceTeam(oldName.c_str(), newName.c_str());
                break;
            }
            case 4:
                teamRegistration->endRegistration("teams.csv");
                isRegistrationActive = false;
                cout << "Team registration closed. Ready for tournament!\n";
                break;
            case 5:
                cout << "Total registered teams: " << totalRegisteredTeams << "\n";
                cout << "Registration status: " << (isRegistrationActive ? "Open" : "Closed") << "\n";
                break;
            case 6:
                cout << "Returning to main menu...\n";
                break;
            default:
                cout << "Invalid option!\n";
        }
        
        if (choice != 6) waitForUserInput();
        
    } while (choice != 6);
}

void APUECIntegratedSystem::handlePlayerRegistrationMenu() {
    int choice;
    string name, playerID;
    bool isEarly;
    
    do {
        cout << "\n=== PLAYER REGISTRATION MANAGEMENT (Task 2B) ===\n";
        cout << "Data Structure Used: Circular Queue, Stack for Wildcards\n";
        cout << string(60, '-') << "\n";
        cout << "1. Register Player\n";
        cout << "2. Add Wildcard Entry\n";
        cout << "3. Check-in Player\n";
        cout << "4. Withdraw Player\n";
        cout << "5. Display All Queues\n";
        cout << "6. Display Checked-in Players\n";
        cout << "7. Back to Main Menu\n";
        cout << "Choice: ";
        cin >> choice;
        cin.ignore();
        
        switch (choice) {
            case 1:
                cout << "Enter player name: ";
                getline(cin, name);
                cout << "Enter player ID: ";
                getline(cin, playerID);
                cout << "Early-bird registration? (1=Yes, 0=No): ";
                cin >> isEarly;
                cin.ignore();
                if (playerRegistration->registerPlayer(name, playerID, isEarly)) {
                    totalRegisteredPlayers++;
                }
                break;
            case 2:
                cout << "Enter wildcard player name: ";
                getline(cin, name);
                cout << "Enter player ID: ";
                getline(cin, playerID);
                playerRegistration->addWildcardEntry(name, playerID);
                break;
            case 3:
                cout << "Enter player ID to check in: ";
                getline(cin, playerID);
                playerRegistration->checkInPlayer(playerID);
                break;
            case 4:
                cout << "Enter player ID to withdraw: ";
                getline(cin, playerID);
                if (playerRegistration->withdrawPlayer(playerID)) {
                    totalRegisteredPlayers--;
                }
                break;
            case 5:
                playerRegistration->displayAllQueues();
                break;
            case 6:
                playerRegistration->displayCheckedInPlayers();
                break;
            case 7:
                cout << "Returning to main menu...\n";
                break;
            default:
                cout << "Invalid option!\n";
        }
        
        if (choice != 7) waitForUserInput();
        
    } while (choice != 7);
}

void APUECIntegratedSystem::handleSpectatorManagementMenu() {
    int choice;
    
    do {
        cout << "\n=== SPECTATOR MANAGEMENT SYSTEM (Task 3) ===\n";
        cout << "Data Structure Used: Priority Queue (Max Heap Implementation)\n";
        
        cout << string(65, '-') << "\n";
        cout << "1. Register New Spectator\n";
        cout << "2. Allocate Seating (Process Priority Queue)\n";
        cout << "3. Display Waiting Queue\n";
        cout << "4. Display Seated Spectators\n";
        cout << "5. Display Venue Status\n";
        cout << "6. Display System Statistics\n";
        cout << "7. Save Spectator Data\n";
        cout << "8. Back to Main Menu\n";
        cout << "Choice: ";
        cin >> choice;
        
        switch (choice) {
            case 1:
                spectatorSystem->registerSpectator();
                totalSpectators++;
                break;
            case 2:
                spectatorSystem->allocateSeating();
                break;
            case 3:
                spectatorSystem->displayWaitingQueue();
                break;
            case 4:
                spectatorSystem->displaySeatedSpectators();
                break;
            case 5:
                spectatorSystem->displayVenueStatus();
                break;
            case 6:
                spectatorSystem->displayStatistics();
                break;
            case 7: {
                string filename;
                cout << "Enter filename (e.g., spectators.csv): ";
                cin >> filename;
                spectatorSystem->saveToFile(filename);
                break;
            }
            case 8:
                cout << "Returning to main menu...\n";
                break;
            default:
                cout << "Invalid option!\n";
        }
        
        if (choice != 8) waitForUserInput();
        
    } while (choice != 8);
}

void APUECIntegratedSystem::handleMatchSchedulingMenu() {
    int choice;
    
    do {
        cout << "\n=== MATCH SCHEDULING & TOURNAMENT (Task 1) ===\n";
        cout << "Data Structure Used: Stack, Queue, Priority Queue, Circular Queue\n";
        cout << string(65, '-') << "\n";
        cout << "1. Start Tournament (Process Teams)\n";
        cout << "2. View Tournament Status\n";
        cout << "3. Back to Main Menu\n";
        cout << "Choice: ";
        cin >> choice;
        
        switch (choice) {
            case 1:
                if (!isRegistrationActive) {
                    cout << "Starting tournament with registered teams...\n";
                    matchScheduler->startTournament("teams.csv");
                    isTournamentActive = true;
                    cout << "Tournament completed! Check Statistics menu for results.\n";
                } else {
                    cout << "Error: Please close team registration first!\n";
                }
                break;
            case 2:
                cout << "Tournament Status: " << (isTournamentActive ? "Completed" : "Not Started") << "\n";
                cout << "Registration Status: " << (isRegistrationActive ? "Open" : "Closed") << "\n";
                break;
            case 3:
                cout << "Returning to main menu...\n";
                break;
            default:
                cout << "Invalid option!\n";
        }
        
        if (choice != 3) waitForUserInput();
        
    } while (choice != 3);
}

void APUECIntegratedSystem::handleStatisticsMenu() {
    int choice;
    string teamName;
    
    do {
        cout << "\n=== STATISTICS & MATCH HISTORY (Task 4) ===\n";
        cout << "Data Structure Used: Stack (LIFO for Recent Match History)\n";
        cout << string(60, '-') << "\n";
        cout << "1. Display All Matches\n";
        cout << "2. Display Matches for Specific Team\n";
        cout << "3. Display Team Statistics\n";
        cout << "4. Back to Main Menu\n";
        cout << "Choice: ";
        cin >> choice;
        cin.ignore();
        
        switch (choice) {
            case 1:
                displayAllMatches();
                break;
            case 2:
                cout << "Enter team name (e.g., Team01): ";
                getline(cin, teamName);
                displayTeamMatches(teamName);
                break;
            case 3:
                cout << "Enter team name (e.g., Team01): ";
                getline(cin, teamName);
                displayTeamStats(teamName);
                break;
            case 4:
                cout << "Returning to main menu...\n";
                break;
            default:
                cout << "Invalid option!\n";
        }
        
        if (choice != 4) waitForUserInput();
        
    } while (choice != 4);
}

void APUECIntegratedSystem::generateSystemReport() {
    cout << "\n" << string(70, '=') << "\n";
    cout << "             APUEC SYSTEM COMPREHENSIVE REPORT\n";
    cout << string(70, '=') << "\n";
    
    // Registration Statistics
    cout << "\n--- REGISTRATION SUMMARY ---\n";
    cout << "Total Teams Registered: " << totalRegisteredTeams << "\n";
    cout << "Total Players Registered: " << totalRegisteredPlayers << "\n";
    cout << "Total Spectators: " << totalSpectators << "\n";
    
    // System Status
    cout << "\n--- SYSTEM STATUS ---\n";
    cout << "Team Registration: " << (isRegistrationActive ? "OPEN" : "CLOSED") << "\n";
    cout << "Tournament: " << (isTournamentActive ? "COMPLETED" : "PENDING") << "\n";
    cout << "Spectator System: " << (isSpectatorSystemActive ? "ACTIVE" : "INACTIVE") << "\n";
    
    // Data Structures Used
    cout << "\n--- DATA STRUCTURES IMPLEMENTATION ---\n";
    cout << "Task 1 (Match Scheduling):\n";
    cout << "  - Stack: Knockout round management\n";
    cout << "  - Queue: Match progression\n";
    cout << "  - Priority Queue: Team seeding\n";
    cout << "  - Circular Queue: Group stage rotation\n";
    
    cout << "\nTask 2 (Registration):\n";
    cout << "  - Queue: Team/Player registration order\n";
    cout << "  - Stack: Wildcard entries (LIFO)\n";
    cout << "  - Priority Queue: Early bird prioritization\n";
    cout << "  - Circular Queue: Check-in processing\n";
    
    cout << "\nTask 3 (Spectator Management):\n";
    cout << "  - Priority Queue: VIP/Influencer/General prioritization\n";
    cout << "  - Max Heap Implementation: Efficient priority processing\n";
    
    cout << "\nTask 4 (Statistics):\n";
    cout << "  - Stack: Recent match history (LIFO access)\n";
    cout << "  - Template Stack: Generic match record storage\n";
    
    // Performance Metrics
    cout << "\n--- SYSTEM PERFORMANCE ---\n";
    cout << "Priority Queue Operations: O(log n) efficiency\n";
    cout << "Stack Operations: O(1) efficiency\n";
    cout << "Queue Operations: O(1) efficiency\n";
    cout << "Memory Management: Dynamic allocation used\n";
    
    cout << "\n" << string(70, '=') << "\n";
    
    // Save report to file
    ofstream reportFile("APUEC_System_Report.txt");
    if (reportFile.is_open()) {
        reportFile << "APUEC System Report Generated\n";
        reportFile << "Teams: " << totalRegisteredTeams << "\n";
        reportFile << "Players: " << totalRegisteredPlayers << "\n";
        reportFile << "Spectators: " << totalSpectators << "\n";
        reportFile.close();
        cout << "Report saved to APUEC_System_Report.txt\n";
    }
}

void APUECIntegratedSystem::waitForUserInput() {
    cout << "\nPress Enter to continue...";
    cin.ignore();
    cin.get();
}

void APUECIntegratedSystem::runSystem() {
    int choice;
    
    cout << "Welcome to APUEC Integrated Tournament Management System!\n";
    displayTaskCredits();
    
    do {
        displayMainMenu();
        cin >> choice;
        
        switch (choice) {
            case 1:
                handleTeamRegistrationMenu();
                break;
            case 2:
                handlePlayerRegistrationMenu();
                break;
            case 3:
                handleSpectatorManagementMenu();
                break;
            case 4:
                handleMatchSchedulingMenu();
                break;
            case 5:
                handleStatisticsMenu();
                break;
            case 6:
                cout << "\n=== TOURNAMENT CONTROL CENTER ===\n";
                cout << "Registration Active: " << (isRegistrationActive ? "YES" : "NO") << "\n";
                cout << "Tournament Status: " << (isTournamentActive ? "COMPLETED" : "PENDING") << "\n";
                cout << "Use individual menus to control each subsystem.\n";
                waitForUserInput();
                break;
            case 7:
                generateSystemReport();
                waitForUserInput();
                break;
            case 8:
                displayTaskCredits();
                waitForUserInput();
                break;
            case 9:
                cout << "Thank you for using APUEC Tournament Management System!\n";
                cout << "System developed by Data Structures Team - CT077-3-2-DSTR\n";
                break;
            default:
                cout << "Invalid choice! Please select 1-9.\n";
        }
        
    } while (choice != 9);
}