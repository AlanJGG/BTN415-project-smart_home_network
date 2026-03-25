# Inventory Management System Protocol

## 1. Overview

Communication uses:
- TCP socket connection using IPv4 (127.0.0.1:65432)
- JSON formatted messages
- Request/Response model

---

## 2. Message Format

All client requests must be valid JSON objects.

### Request Format

{
    "action": "string",
    "username": "string",
    "password": "string (required for login only)",
    "upc": "string (required for purchase only)",
    "quantity" integer (required for purchase only)"
}

The "action" field determines the operation

---

## 3. Response Format

All server responses follow this structure:

{
    "status": "ok | error",
    "message": "string (optional)",
    "data": object (optional)
}

---

## 4. Supported Actions

### 4.1 Login

Request:
{
    "action": "login",
    "username": "user1",
    "password": "12345"
}

Response (Success):
{
    "status": "ok",
    "message": "Login Successful"
}

Reponse (Failure):
{
    "status": "error",
    "message": "Invalid credentials"
}

---

### 4.2 View Inventory

Request:
{
    "action": "view_inventory",
    "username": "user1"
}

Response:
{
    "status": "ok",
    "data": {
        "1001": 10,
        "1002": 5,
        "1003": 20
    }
}

If user is not logged in:
{
    "status": "error",
    "message": "Login required"
}

---

### 4.3 Purchase Item

Request:
{
    "action": "purchase",
    "username": "user1",
    "upc": "1001",
    "quantity": 2
}

Response (Success):
{
    "status": "ok",
    "message": "Purchased 2 of item 1001"
}

Response (Failure):
{
    "status": "error",
    "message": "Login required | Item not found | Not enough stock"
}

---

### 4.4 Logout

Request:
{
    "action": "logout",
    "username": "user1"
}

Response:
{
    "status": "ok",
    "message": "Logged out"
}

---

## 5. Persistence

The server persists:
- Inventory levels (server_inventory.json)
- Logged-in users (server_logins.json)

Client persists:
- Local inventory (local_inventory.json)

Data is saved during shutdown, files are read and restored during startup. 