# MyList AI

**MyList AI** is a modern desktop To-Do List application built using **C++** and the **Qt Framework**, featuring an integrated **local AI assistant** powered by **llama.cpp**.

Unlike cloud-based AI assistants, MyList AI allows users to run AI models **completely offline** on their own computer. Users can configure their preferred **llama-server** executable and **GGUF AI model** directly from the application, making the AI system flexible and customizable.

The application combines task management, reminders, desktop notifications, and AI-powered assistance into a single productivity application.

---

# Features

## User Authentication

* Secure User Registration
* User Login System
* Session Management

---

## Task Management

* Create Tasks
* Edit Task Details
* Delete Tasks
* Search Tasks
* Pending / Completed Task Filtering
* Task Priority Support
* Due Date Management

---

## Reminder System

* Automatic Reminder Alerts
* Deadline Tracking

---

## Overdue Desktop Notifications

* Native Desktop Notifications using **QSystemTrayIcon**
* Displays overdue tasks even while using the application

---

## Task Export

* Export all user tasks into a **TXT** file

---

## AI Assistant

The built-in AI Assistant understands the user's current task list and answers questions using a locally running Large Language Model.

Example prompts:

* *What are my pending tasks?*
* *Which task should I complete first?*
* *Summarize my current workload.*
* *Give me a study plan based on my tasks.*
* *Explain today's schedule.*

The AI receives the user's current tasks as context before generating a response, allowing it to provide personalized answers.

---

## Configurable AI Environment

One of the main features of **MyList AI** is that users are **not limited to a fixed AI model**.

Users can configure:

* Path to **llama-server.exe**
* Path to any compatible **GGUF AI model**
* Maximum response token count

This allows users to experiment with different local models depending on their hardware and requirements.

---

# Technologies Used

* C++
* Qt Framework
* SQLite
* Qt Style Sheets (QSS)
* CMake
* llama.cpp
* GGUF AI Models

---

# Screenshots

## Login

![Login](screenshots/Login.png)

---

## Dashboard

![Dashboard](screenshots/Dashboard.png)

---

## User Profile

![Profile](screenshots/profile.png)

---

## Task Details

![Task Details](screenshots/task_details.png)

---

## AI Assistant

*(Add screenshot here)*

---

## AI Settings

*(Add screenshot here)*

---

# Project Structure

```text
MYLIST_AI/
│
├── Forms/
├── Headers/
├── Sources/
├── Styles/
├── icons/
│
├── resources.qrc
├── CMakeLists.txt
├── README.md
└── .gitignore
```

---

# Main Modules

## main.cpp

Application entry point.

Initializes the Qt application and launches the main window.

---

## mainwindow.cpp

Contains the primary application logic.

Responsibilities include:

* User Interface
* Task Management
* Navigation
* Reminder System
* Desktop Notifications
* Opening the AI Assistant

---

## authmanager.cpp

Handles:

* User Registration
* Login Authentication
* Current User Management

---

## taskmanager.cpp

Handles all task-related database operations.

Responsibilities include:

* Create
* Read
* Update
* Delete
* Search
* Filter
* Statistics

---

## databasemanager.cpp

Responsible for:

* SQLite Connection
* Database Initialization
* Table Creation

---

## taskdetailsdialog.cpp

Displays complete information about a selected task.

---

## userprofile.cpp

Displays user information and exports task data into TXT format.

---

# AI System Architecture

The AI Assistant is divided into three independent modules.

---

## aidialog.cpp

Acts as the user interface for the AI Assistant.

Responsibilities:

* Receives user questions
* Collects current task data
* Builds a structured prompt
* Sends the prompt to AIManager
* Displays AI responses

Example workflow:

```
User Question
        ↓
Create Prompt
        ↓
Send to AIManager
        ↓
Receive AI Response
        ↓
Display Response
```

---

## aimanager.cpp

The core communication layer between the application and **llama-server**.

Responsibilities:

* Starts llama-server.exe
* Checks server health
* Sends prompts
* Receives AI responses
* Parses JSON replies
* Emits Qt signals back to the UI

### Qt Classes Used

### QProcess

Starts and manages **llama-server.exe** as a separate process from the application.

---

### QNetworkAccessManager

Handles all HTTP communication with the local AI server.

---

### QNetworkRequest

Builds HTTP requests such as:

* `/completion`
* `/health`

before sending them to the server.

---

### QNetworkReply

Receives the HTTP response returned by llama-server and reports network errors if they occur.

---

### QJsonDocument

Converts JSON data between Qt objects and raw JSON text.

Used for both sending prompts and reading AI responses.

---

### QByteArray

Stores the raw byte data received from the AI server before it is converted into JSON.

---

### QSettings

Reads user-configured settings including:

* llama-server path
* GGUF model path
* Maximum AI response tokens

These settings persist even after restarting the application.

---

### Qt Signals & Slots (`connect()`)

The AI system is fully asynchronous.

The UI never freezes while waiting for the AI.

Workflow:

```
Send Prompt
      ↓
AI Processing
      ↓
Signal Emitted
      ↓
Dialog Updates Automatically
```

---

## appsettings.cpp

Provides a graphical settings dialog where users configure their local AI environment.

Features:

* Browse for llama-server.exe
* Browse for GGUF model
* Configure maximum token count
* Save settings using QSettings

This design allows the application to support different AI models without modifying the source code.

---

# AI Workflow

```
User Question
        ↓
aidialog.cpp
        ↓
Prompt Generation
        ↓
AIManager
        ↓
QNetworkRequest
        ↓
llama-server
        ↓
Local GGUF Model
        ↓
JSON Response
        ↓
QNetworkReply
        ↓
aidialog.cpp
        ↓
Display Answer
```

---

# Future Improvements

Planned features include:

* Natural Language Task Creation
* AI Task Prioritization
* Smart Reminder Suggestions
* Productivity Analytics
* AI Calendar Planning
* Voice Interaction
* Multi-model Support
* Streaming AI Responses

---

# Author

**Jibin George**

Bachelor of Computer Applications (BCA)

Built using **C++**, **Qt Framework**, **SQLite**, and **llama.cpp**.
