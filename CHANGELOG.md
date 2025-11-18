# Changelog

## [Unreleased]

### Added
- Comprehensive exception hierarchy with new exception groups:
  - TaskHandlerException and subclasses for task handling errors
  - WindowHandlerException and subclasses for window management errors
  - EngineException and subclasses for engine lifecycle errors
  - TaskException and subclasses for task logic errors
- Extensive logging throughout the codebase using HARMONY_* macros
- Comprehensive assertions for preconditions and invariants
- New WindowHandler getters and setters for enhanced configuration
- TasksHandler::getActiveWorkerCount() function to query active worker threads

### Changed
- Updated TasksHandler with extensive logging, assertions, and proper exception handling
- Enhanced WindowHandler constructor to support more configuration options
- Improved exception handling in all core components (Engine, Task, Clock, Color)
- Updated EXCEPTIONS_README.md with complete documentation

### Fixed
- Potential null pointer dereferences in TasksHandler
- Improved thread safety in TasksHandler worker pool
- Better error messages and diagnostics throughout the codebase
