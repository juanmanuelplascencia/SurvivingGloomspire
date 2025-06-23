# Code Review Process

## Overview
This document outlines the code review process for Surviving Gloomspire, ensuring high code quality, knowledge sharing, and consistency across the codebase.

## Review Guidelines

### 1. When to Review
- All feature branches before merging to `main`
- Hotfixes before deployment
- Major refactoring changes
- Security-sensitive code

### 2. Review Checklist

#### Code Quality
- [ ] Follows project coding standards
- [ ] No commented-out code
- [ ] Proper error handling
- [ ] Memory management
- [ ] Thread safety

#### Architecture
- [ ] Follows established patterns
- [ ] No unnecessary coupling
- [ ] Proper separation of concerns
- [ ] Scalability considered

#### Performance
- [ ] No obvious performance issues
- [ ] Efficient algorithms/data structures
- [ ] Minimal memory allocations
- [ ] Async operations handled properly

#### Testing
- [ ] Unit tests added/updated
- [ ] Test coverage adequate
- [ ] Edge cases considered
- [ ] Test data appropriate

### 3. Review Process

#### Submitter
1. Create a feature branch from `main`
2. Make changes with atomic commits
3. Ensure all tests pass
4. Update documentation
5. Create pull request (PR)
   - Link related issues
   - Add description of changes
   - Tag reviewers

#### Reviewer
1. Acknowledge review request within 24h
2. Perform review using checklist
3. Leave clear, constructive feedback
4. Approve, request changes, or discuss

#### After Review
1. Address all feedback
2. Push updates
3. Request re-review if needed
4. Squash and merge when approved

## Tools

### 1. Pull Request Template
```markdown
## Description
[Brief description of changes]

## Related Issues
- Fixes #123
- Implements #456

## Testing
- [ ] Unit tests added/updated
- [ ] Manual testing performed
- [ ] Performance impact analyzed

## Documentation
- [ ] Code comments updated
- [ ] Documentation updated
- [ ] Changelog entry added
```

### 2. Automation
- **Static Analysis**: SonarQube
- **Code Formatting**: clang-format
- **Build Verification**: GitHub Actions
- **Test Coverage**: Codecov

## Best Practices

### For Submitters
1. Keep PRs focused and small (<400 LOC)
2. Write clear commit messages
3. Self-review before submitting
4. Respond promptly to feedback

### For Reviewers
1. Be respectful and constructive
2. Focus on code, not coder
3. Explain "why" not just "what"
4. Acknowledge good patterns

## Common Issues

### 1. Style Violations
- **Issue**: Inconsistent formatting
- **Solution**: Use .clang-format and pre-commit hooks

### 2. Missing Tests
- **Issue**: New code lacks test coverage
- **Solution**: Require tests for all new features

### 3. Over-engineering
- **Issue**: Premature optimization
- **Solution**: Keep it simple, YAGNI

## Review Metrics

### 1. Cycle Time
- **Target**: < 24h for initial review
- **Goal**: 85% of PRs reviewed within 24h

### 2. Review Depth
- **Comments per KLOC**: 5-15
- **Comment Resolution**: 95%+

### 3. Code Churn
- **Target**: < 15% of lines changed after review
- **Goal**: Reduce rework through better upfront design

## Continuous Improvement

### 1. Retrospectives
- Frequency: Monthly
- Duration: 1 hour
- Focus: Process improvements

### 2. Training
- New hire onboarding
- Code review workshops
- Knowledge sharing sessions

---
*Document Version: 1.0*  
*Last Updated: 2025-06-23*
