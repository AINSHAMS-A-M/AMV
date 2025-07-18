# Voting System Project

## Overview
A C++ voting application with Qt integration that manages users, polls, and voting records with CSV data persistence.

## Key Features

### User Management
- Account creation with secure password hashing
- User authentication (login)
- Profile editing (username, personal info, password changes)
- Data validation for all user inputs

### Poll System
- Create polls with multiple options
- Modify existing poll options
- Close/open polls
- View real-time voting results
- Prevent voting in own polls

### Data Management
- CSV file storage (users, polls, options, votes)
- Automatic data directory creation
- Custom `MeshVector` container for in-memory storage
- Data validation and error handling

## Code Structure
src/
├── core/
│ ├── data_structures.hpp # Custom MeshVector implementation
│ ├── db.hpp # Database interface
│ └── _db_structs.hpp # Core data structures
├── services/
│ ├── hash.[hpp/cpp] # Password hashing
│ ├── poll.cpp # Poll logic
│ ├── user.cpp # User management
│ └── data_persistence.cpp # CSV read/write
├── _structs.hpp # DTO structures
└── services.hpp # Public API

## Getting Started

### Prerequisites
- C++17 compatible compiler
- Qt development libraries
- CMake (recommended)

### Building
1. Clone the repository
2. Create build directory: `mkdir build && cd build`
3. Run CMake: `cmake ..`
4. Compile: `make`

## Configuration
The application automatically creates a `data/` directory containing:

- `users.csv` - User accounts
- `polls.csv` - Poll definitions  
- `pollOptions.csv` - Voting options
- `userVotes.csv` - Voting records

## API Usage

Key functions available in `services.hpp`:

```cpp
// User management
std::string create_user(CreateUser user); 
std::string log_in(std::string username, std::string password);

// Poll management  
Poll create_poll(CreatePoll poll);
void delete_poll(size_t poll_id);

// Voting
std::string create_user_vote(size_t user_id, size_t poll_id, size_t option_id);
RetrievePollResultAdmin retrieve_poll_results(size_t poll_id);

## Security
- Password hashing with user-specific salts  
- Input sanitization  
- Bounds checking on all container operations  
- File operation error handling  

## Limitations
- CSV storage is not optimized for large datasets  
- Single-user desktop application  
- No built-in backup system  

## Future Improvements
- SQLite database backend  
- Network/multi-user support  
- Web administration interface  
- Enhanced reporting features  
