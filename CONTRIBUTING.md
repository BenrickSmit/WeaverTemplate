# Contributing to Weaver GUI Project

We welcome contributions to the Weaver GUI Project! By participating in this project, you agree to abide by our [Code of Conduct](CODE_OF_CONDUCT.md).

## How to Contribute

### Reporting Bugs

If you find a bug, please open an issue on our [issue tracker](link-to-your-issue-tracker-here) and provide the following information:

*   A clear and concise description of the bug.
*   Steps to reproduce the behavior.
*   Expected behavior.
*   Screenshots or error messages, if applicable.
*   Your operating system and compiler version.

### Suggesting Enhancements

If you have an idea for a new feature or an improvement, please open an issue on our [issue tracker](link-to-your-issue-tracker-here) and describe your suggestion. Explain why it would be a valuable addition to the project.

### Contributing Code

We appreciate code contributions! To contribute code, please follow these steps:

1.  **Fork the repository** and clone it to your local machine.
2.  **Create a new branch** for your feature or bug fix: `git checkout -b feature/your-feature-name` or `git checkout -b bugfix/issue-description`.
3.  **Make your changes.** Ensure your code adheres to the project's coding style.
    *   **Code Style:** We use `clang-format` for code formatting and `clang-tidy` for static analysis. A pre-commit hook is provided to help enforce these standards. Please ensure you have `clang-format` and `clang-tidy` installed and in your PATH.
    *   **Pre-commit Hook:** To enable the pre-commit hook, copy `scripts/pre-commit.bat` (for Windows) or `scripts/pre-commit.sh` (for Linux/macOS) to `.git/hooks/pre-commit`. For `clang-tidy` to work correctly, ensure you have generated `compile_commands.json` by running a full build (e.g., `build.bat` or `build.sh`) before committing.
4.  **Write tests** for your changes, if applicable. All new features and bug fixes should have corresponding tests.
5.  **Run tests:** Ensure all existing tests pass. You can run tests using `run_tests.bat` (Windows) or `run_tests.sh` (Linux/macOS).
6.  **Commit your changes** with a clear and concise commit message.
7.  **Push your branch** to your forked repository.
8.  **Open a Pull Request** to the `main` branch of the upstream repository. Provide a clear description of your changes and reference any related issues.

## Code Style

We use `.clang-format` and `.clang-tidy` configuration files to maintain a consistent code style and quality. Please ensure your contributions adhere to these standards.

## License

By contributing to this project, you agree that your contributions will be licensed under its Apache 2.0 License.
