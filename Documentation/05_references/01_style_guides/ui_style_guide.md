# UI Style Guide

## Design Principles

### Core Principles
1. **Clarity**: Interface should be self-explanatory
2. **Consistency**: Similar elements behave similarly
3. **Efficiency**: Common actions should be quick to perform
4. **Feedback**: Users should understand the result of their actions
5. **Accessibility**: Usable by people with disabilities

## Visual Language

### Color Palette

#### Primary Colors
- Primary: `#2B579A` (Dark Blue)
- Secondary: `#F25022` (Orange)
- Accent: `#7FBA00` (Green)

#### UI Colors
- Background: `#1E1E1E` (Dark Gray)
- Surface: `#252526` (Lighter Gray)
- Text: `#FFFFFF` (White)
- Text Secondary: `#A0A0A0` (Light Gray)
- Error: `#F44336` (Red)
- Success: `#4CAF50` (Green)
- Warning: `#FFC107` (Yellow)

### Typography

#### Font Family
- Primary: [Roboto](https://fonts.google.com/specimen/Roboto)
- Monospace: [Roboto Mono](https://fonts.google.com/specimen/Roboto+Mono)

#### Font Sizes
- H1: 32px (2rem)
- H2: 24px (1.5rem)
- H3: 20px (1.25rem)
- Body: 16px (1rem)
- Small: 14px (0.875rem)
- Tiny: 12px (0.75rem)

### Spacing
Base unit: 8px
- XS: 4px (0.25rem)
- SM: 8px (0.5rem)
- MD: 16px (1rem)
- LG: 24px (1.5rem)
- XL: 32px (2rem)
- XXL: 48px (3rem)

### Icons
- Use [Material Icons](https://fonts.google.com/icons)
- Standard size: 24px
- Small size: 16px
- Large size: 32px

## UI Components

### Buttons

#### Primary Button
- Background: Primary Color
- Text: White
- Padding: 12px 24px
- Border Radius: 4px
- Hover: 10% lighter
- Active: 20% darker

#### Secondary Button
- Background: Transparent
- Border: 1px solid Primary Color
- Text: Primary Color
- Hover: 10% background opacity

### Input Fields
- Height: 40px
- Padding: 0 12px
- Border: 1px solid #444
- Border Radius: 4px
- Focus: 2px solid Primary Color

### Dropdowns
- Match input field styling
- Arrow indicator on right
- Max height: 300px with scroll

### Checkboxes & Radio Buttons
- Size: 20px × 20px
- Border: 2px solid #444
- Checked: Primary color fill
- Hover: Lighten border color

## Layout

### Grid System
- 12-column responsive grid
- Gutter width: 24px
- Max content width: 1200px
- Side padding: 24px on mobile, 32px on desktop

### Breakpoints
- Mobile: < 768px
- Tablet: 768px - 1023px
- Desktop: ≥ 1024px

## Animations

### Durations
- Fast: 100ms
- Normal: 200ms
- Slow: 300ms

### Easing
- Standard: `cubic-bezier(0.4, 0, 0.2, 1)`
- Deceleration: `cubic-bezier(0, 0, 0.2, 1)`
- Acceleration: `cubic-bezier(0.4, 0, 1, 1)`
- Sharp: `cubic-bezier(0.4, 0, 0.6, 1)`

## Accessibility

### Color Contrast
- Normal text: 4.5:1 minimum
- Large text: 3:1 minimum
- UI components: 3:1 minimum

### Focus States
- Visible focus indicator
- High contrast outline
- No `outline: none` without alternative

### Keyboard Navigation
- Logical tab order
- Skip to main content link
- Accessible menus and dialogs

## Unreal UMG Specific

### Widget Naming
- `WBP_` prefix for widget blueprints
- `IMG_` for images
- `TXT_` for text blocks
- `BTN_` for buttons
- `CB_` for checkboxes
- `DD_` for dropdowns

### Common Widgets
1. **Main Menu**
   - Game title
   - Play button
   - Options
   - Quit

2. **HUD**
   - Health/Stamina
   - Minimap
   - Objectives
   - Quick items

3. **Inventory**
   - Grid layout
   - Item tooltips
   - Category filters

## Localization
- All text in string tables
- Support for RTL languages
- 30% extra space for translations

## Performance
- Use widget pooling for lists
- Virtualize long lists
- Lazy load assets
- Optimize draw calls

## Testing
### Visual Regression
- Test at all breakpoints
- Verify on different DPIs
- Check color contrast

### Functional Testing
- Keyboard navigation
- Screen reader compatibility
- Gamepad support

---
*Document Version: 1.0*  
*Last Updated: 2025-06-23*
