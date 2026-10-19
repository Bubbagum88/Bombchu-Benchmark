# CHANGELOG
All notable changes to this project will be documented in this file.

The format is based on Keep a Changelog (https://keepachangelog.com/en/1.0.0/)
and this project adheres to Semantic Versioning.

## [Unreleased]
### Added
- Homebrew Channel `meta.xml` and `icon.png` in `hbc/`; CI packages them with the Wii build as `apps/bombchu/`
- Pushing a `v*` tag publishes a GitHub release with the Wii zip and both `.dol` files
- README sections on downloading, running, controls, requirements and result accuracy, plus build and license badges

### Planned
- Network benchmarking module
- CPU benchmarking (PowerPC)
- Filesystem benchmarking module
- USB storage support
- GX-based UI engine

## [2026-10-19] Working Build, libogc2 and Repository Tidy
### Added
- GameCube build target (`make PLATFORM=gc`) alongside the Wii build
- CI builds both platforms, uploads `bombchu-wii.dol` and `bombchu-gc.dol`, and can be run by hand
- Benchmark selector and results screen reachable from the main menu
- `.gitattributes` to keep LF line endings

### Changed
- Builds against libogc2 by default; CI installs it from the libogc2 package repository
- Makefile rewritten: object files go in `build/<platform>`, header changes trigger rebuilds
- Themes use console colours until a GX renderer exists
- Menus only redraw when input changes
- Image assets sorted into `images/source/` and `images/previews/`
- `contributions.md` renamed to `CONTRIBUTING.md`, `CHANGELOG` to `CHANGELOG.md`

### Fixed
- Build failures from headers removed with `include/`, duplicate and missing symbols, and missing link libraries

### Removed
- `ui/ui_screens.c`, an older duplicate of the theme selector

## [2026-07-06] Repository Cleanup & Asset Organization
### Added
- Added full Bombchu ASCII set to `images/bombchu/`
- Added full Title ASCII set to `images/title/`
- Added PNG/WebP assets to `images/`
- Added `ui/ui_theme_selector.h` for future UI system

### Changed
- Updated README license from GPLv2 → GPLv3
- Updated repository structure for UI asset management
- Refactored `.gitignore` to streamline ignored files
- Updated README to reflect project direction

### Removed
- Removed legacy root-level ASCII files
- Removed placeholder `main()` and video setup code from `src/`

## [2026-07-05] Initial Project Setup
### Added
- Initial repository structure
- Makefile for building Bombchu Benchmark
- LICENSE (GPLv3)
- Contribution guidelines
- Basic README with project overview
