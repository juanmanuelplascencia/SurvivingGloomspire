# UI Style Guide

## Overview
This document defines the visual and interactive standards for the game's user interface to ensure consistency and a polished user experience.

## Design Principles

### 1. Clarity
- Clear visual hierarchy
- Readable typography
- Intuitive navigation

### 2. Consistency
- Uniform styling across all screens
- Standardized components
- Predictable interactions

### 3. Feedback
- Visual feedback for all actions
- Clear error messages
- Loading indicators

## Visual Style

### Color Palette

#### Primary Colors
- Primary: `#2B579A` (Dark Blue)
- Secondary: `#F25022` (Orange)
- Accent: `#7FBA00` (Green)

#### UI Colors
- Background: `#1E1E1E` (Dark Gray)
- Surface: `#2D2D30` (Lighter Gray)
- Text Primary: `#FFFFFF` (White)
- Text Secondary: `#B0B0B0` (Light Gray)
- Error: `#FF4444` (Red)
- Success: `#00C851` (Green)
- Warning: `#FFBB33` (Yellow)
- Info: `#33B5E5` (Blue)

### Typography

#### Font Family
- Primary: Roboto
- Secondary: Open Sans

#### Font Sizes
- H1: 32px (Bold)
- H2: 24px (Semi-Bold)
- H3: 20px (Medium)
- Body: 16px (Regular)
- Caption: 14px (Light)
- Button: 16px (Medium)

### Spacing
- Base Unit: 8px
- Small: 4px
- Medium: 8px
- Large: 16px
- X-Large: 24px
- XX-Large: 32px

## UI Components

### Buttons

#### Primary Button
- Background: Primary Color
- Text: White
- Padding: 12px 24px
- Border Radius: 4px
- Hover: 10% lighter
- Active: 10% darker

#### Secondary Button
- Background: Transparent
- Border: 1px solid Primary Color
- Text: Primary Color
- Padding: 12px 24px
- Border Radius: 4px

### Input Fields

#### Text Input
- Background: Surface Color
- Border: 1px solid #444
- Border Radius: 4px
- Padding: 12px
- Placeholder: 60% opacity text
- Focus: 2px solid Primary Color

#### Dropdown
- Background: Surface Color
- Border: 1px solid #444
- Border Radius: 4px
- Padding: 12px
- Arrow: 8x8px

### Navigation

#### Menu Items
- Text: White
- Hover: Primary Color
- Active: Primary Color (Darker)
- Padding: 12px 16px

#### Breadcrumbs
- Text: Light Gray
- Separator: /
- Hover: White
- Active: Primary Color

## Layout

### Grid System
- Columns: 12
- Gutter: 16px
- Margin: 16px
- Max Width: 1200px

### Responsive Breakpoints
- Mobile: < 768px
- Tablet: 768px - 1024px
- Desktop: > 1024px

## Animations

### Transitions
- Duration: 200ms
- Easing: Ease-in-out

### Micro-interactions
- Button Press: Scale 95%
- Hover: Slight scale up (102%)
- Focus: Glow effect

## Accessibility

### Text Contrast
- Normal Text: 4.5:1 minimum
- Large Text: 3:1 minimum
- UI Components: 3:1 minimum

### Focus States
- Visible focus indicator
- Keyboard navigation support
- Screen reader compatibility

## Implementation Guidelines

### Unreal UMG
- Use Common UI plugin for consistent styling
- Implement style assets for reuse
- Use widget blueprints for complex components

### Data Binding
- Use MVVM pattern where applicable
- Implement data validation
- Handle loading states

## Example Widgets

### Health Bar
```
+ Canvas Panel
  + Progress Bar (Health)
    - Fill Color: #FF4444
    - Percent: 75
  + Text Block
    - Text: "75/100"
    - Color: White
    - Font Size: 14
    - Alignment: Center
```

### Inventory Slot
```
+ Border
  - Brush: Rounded Rectangle
  - Color: Surface Color
  + Image (Item Icon)
    - Size: 64x64
  + Text Block (Item Count)
    - Position: Bottom Right
    - Color: White
    - Font Size: 12
```

## Best Practices

### Do's
- Use the design system consistently
- Follow the grid system
- Maintain proper contrast
- Test on multiple devices

### Don'ts
- Don't use more than 3 type sizes per screen
- Avoid pure black or pure white
- Don't overload screens with information
- Avoid custom styling unless necessary

## Revision History

| Version | Date       | Author          | Changes                        |
|---------|------------|-----------------|--------------------------------|
| 1.0     | 2023-11-15 | [Your Name]     | Initial version                |


## Related Documents
- [Content Guidelines](content_guidelines.md)
- [Project Setup Guide](project_setup_guide.md)
- [Performance Guidelines](performance_guidelines.md)

---
*Document Version: 1.0*  
*Last Updated: 2023-11-15*
