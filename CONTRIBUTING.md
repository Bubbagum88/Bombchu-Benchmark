# Contributing to Bombchu Benchmark

Thank you for your interest in improving Bombchu Benchmark. This project is an early-stage system-level benchmarking tool for Nintendo GameCube and Wii homebrew environments. Contributions are welcome, but they should improve clarity, reproducibility, or correctness of benchmarks.

## License

By contributing to this project, you agree that your contributions will be licensed under the GNU General Public License v3.0 or later.

## Types of Contributions

The following types of contributions are especially useful:

- Improvements to benchmark accuracy or consistency
- New benchmark modules (storage, CPU, network, filesystem)
- Bug fixes in measurement logic or hardware interaction
- Performance optimizations that do not distort benchmark results
- Documentation improvements

## Benchmark Integrity

This project is intended to produce reproducible and comparable results.

When modifying benchmark code:

- Do not introduce platform-specific shortcuts that distort results
- Document any changes that affect timing, measurement, or methodology
- Clearly explain why a change improves accuracy or relevance
- Avoid “optimizations” that change what is being measured

If a change affects results, it should also explain how and why.

## Code Style

- Keep changes minimal and focused
- Avoid unrelated refactoring in benchmark-critical code
- Prefer clarity over cleverness
- If behavior changes, document it

## Submitting Changes

Pull requests should include:

- A clear description of what was changed
- Reasoning for the change
- Any expected impact on benchmark results
- If applicable, before/after performance comparison

## Experimental Features

Experimental benchmark modules are allowed, but they must be clearly marked as such and should not replace stable benchmark behavior.

## Final Note

This project exists to measure systems consistently. If a contribution makes results harder to interpret, it is likely not an improvement.
