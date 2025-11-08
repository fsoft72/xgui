# Calendar

The `Calendar` widget displays a calendar-like interface for selecting dates. It derives from `Widget` and provides properties for managing the selected date. This class cannot be directly instantiated, but must be created using the `Master::CreateCalendar()` function.

## Class Hierarchy

```
Object
└── Widget
    └── Calendar
```

## Methods

This widget has no additional methods beyond those inherited from `Widget`.

## Properties

### `day` (Read/Write)

Sets or retrieves the day of the month currently selected in the calendar.

**Type:** Integer (as string)

**Values:** `1` to `31` (depending on the month)

---

### `month` (Read/Write)

Sets or retrieves the month currently selected in the calendar.

**Type:** Integer (as string)

**Values:** `1` to `12`

---

### `year` (Read/Write)

Sets or retrieves the year currently selected in the calendar.

**Type:** Integer (as string)

---

### `date` (Read/Write)

Sets or retrieves the full date currently selected in the calendar.

**Type:** String

**Format:** `"year-month-day"` (e.g., `"2025-11-08"`)

---

## Events

### `onselect` (xgui::TextCallback)

Fired when the user selects a date.

**Parameters:**
- `text` - The selected date in format `"year-month-day"`

---
