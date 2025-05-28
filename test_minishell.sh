#!/bin/bash

MINISHELL="./minishell"
LOG="minishell_test.log"
TMP_OUT=".tmp_out"
TMP_REF=".tmp_ref"
TMP_FILE=".test_file"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Counters
TOTAL_TESTS=0
PASSED_TESTS=0
FAILED_TESTS=0

rm -f "$LOG" "$TMP_OUT" "$TMP_REF" "$TMP_FILE"
touch "$LOG"

echo -e "${BLUE}=== MINISHELL MANDATORY TEST SUITE ===${NC}" | tee -a "$LOG"
echo "Testing only mandatory requirements from subject" | tee -a "$LOG"
echo "Starting tests at $(date)" | tee -a "$LOG"
echo "----------------------------------------" | tee -a "$LOG"

test_case() {
    local cmd="$1"
    local description="$2"
    TOTAL_TESTS=$((TOTAL_TESTS + 1))

    echo -e "\n${YELLOW}TEST $TOTAL_TESTS: $description${NC}" | tee -a "$LOG"
    echo "COMMAND: $cmd" | tee -a "$LOG"

    # Your minishell - add exit to ensure clean termination
    timeout 10s bash -c "echo '$cmd'; echo 'exit'" | $MINISHELL > "$TMP_OUT" 2>/dev/null
    local minishell_exit=$?

    # Reference bash - also add the command input for fair comparison
    timeout 10s bash -c "echo '$cmd'; echo '$cmd' | bash" > "$TMP_REF" 2>/dev/null
    local bash_exit=$?

    # Clean minishell output: remove prompts but keep the command input and output
    sed 's/^minishell\$> //g; /^exit$/d; s/minishell\$> exit$//g' "$TMP_OUT" > "$TMP_OUT.clean"

    # Compare cleaned outputs
    if diff -u "$TMP_REF" "$TMP_OUT.clean" >> "$LOG" 2>/dev/null; then
        echo -e "${GREEN}✅ PASSED${NC}" | tee -a "$LOG"
        PASSED_TESTS=$((PASSED_TESTS + 1))
    else
        echo -e "${RED}❌ FAILED${NC}" | tee -a "$LOG"
        FAILED_TESTS=$((FAILED_TESTS + 1))
        echo "Expected output:" >> "$LOG"
        cat "$TMP_REF" >> "$LOG" 2>/dev/null
        echo "Your cleaned output:" >> "$LOG"
        cat "$TMP_OUT.clean" >> "$LOG" 2>/dev/null
        echo "Your raw output:" >> "$LOG"
        cat "$TMP_OUT" >> "$LOG" 2>/dev/null
    fi
    echo "----------------------------------------" >> "$LOG"
}

# Create test file
echo "test content for input redirection" > "$TMP_FILE"

echo -e "${BLUE}=== BUILT-IN COMMANDS (MANDATORY) ===${NC}"

# echo with -n option
test_case "echo hello world" "Basic echo command"
test_case "echo -n" "Echo -n without arguments"
test_case "echo" "Echo without arguments"

# pwd (no options)
test_case "pwd" "Print working directory"

# cd with relative/absolute paths only
test_case "pwd" "Current directory before cd"
test_case "cd ." "Change to current directory"
test_case "cd .." "Change to parent directory"
test_case "pwd" "Check directory after cd .."
test_case "cd /tmp" "Change to absolute path"
test_case "pwd" "Check we're in /tmp"
test_case "pwd" "Check we're back"
test_case "cd nonexistent_dir" "Change to nonexistent directory"

# export (no options)
test_case "export VAR=hello" "Export variable"
test_case "echo \$VAR" "Use exported variable"
test_case "export VAR=42" "Export numeric variable"
test_case "echo \$VAR" "Use numeric variable"
#test_case "export" "List exported variables"
test_case "export VAR1=test VAR2=hello" "Export multiple variables"

# unset (no options)
test_case "export VAR=test" "Export variable for unset test"
test_case "unset VAR" "Unset exported variable"
test_case "echo \$VAR" "Check variable after unset" #i have prob
test_case "unset NONEXISTENT" "Unset nonexistent variable"
test_case "unset" "Unset without arguments"

# env (no options or arguments)
test_case "env" "Show environment variables"
test_case "export TEST=hello" "Export test variable"
test_case "env | grep TEST" "Check exported variable in env"

# exit (no options)
#test_case "exit" "Exit command"
test_case "exit 42" "Exit with status code"

echo -e "${BLUE}=== EXECUTABLE SEARCH (PATH, RELATIVE, ABSOLUTE) ===${NC}"

test_case "ls" "Command from PATH"
test_case "cat /etc/hosts | head -1" "Absolute path command"
test_case "./nonexistent" "Relative path nonexistent"
test_case "/bin/echo hello" "Absolute path echo"
test_case "which ls" "Which command"
test_case "nonexistent_command" "Command not found"

echo -e "${BLUE}=== QUOTES HANDLING ===${NC}"

# Single quotes - prevent ALL interpretation
test_case "echo 'hello world'" "Single quotes basic"
test_case "echo 'hello \$USER world'" "Single quotes with dollar sign"
test_case "echo 'hello | world'" "Single quotes with pipe"
test_case "echo 'hello > world'" "Single quotes with redirection"
test_case "echo ''" "Empty single quotes"
test_case "echo 'hello'\''world'" "Single quote inside single quotes"

# Double quotes - prevent interpretation except $
test_case 'echo "hello world"' "Double quotes basic"
#test_case 'echo "hello \$USER world"' "Double quotes with variable expansion"
test_case 'echo "hello | world"' "Double quotes with pipe (no interpretation)"
test_case 'echo "hello > world"' "Double quotes with redirection (no interpretation)"
test_case 'echo ""' "Empty double quotes"
#test_case 'echo "hello\"world"' "Escaped quote in double quotes"

echo -e "${BLUE}=== ENVIRONMENT VARIABLES ===${NC}"

test_case "echo \$HOME" "HOME variable expansion"
test_case "echo \$USER" "USER variable expansion"
test_case "echo \$PATH | head -c 50" "PATH variable expansion"
test_case "echo \$NONEXISTENT" "Nonexistent variable"
test_case "export VAR=hello" "Custom variable export"
test_case "echo \$VAR" "Custom variable expansion"
test_case "export VAR=test" "Update variable"
test_case "echo \$VAR world" "Variable with additional text"
test_case "export VAR=hello" "Reset variable"
test_case "echo \$VAR\$VAR" "Multiple variable expansions"
test_case "echo \$" "Lone dollar sign"

echo -e "${BLUE}=== EXIT STATUS (\$?) ===${NC}"

test_case "true" "True command"
test_case "echo \$?" "True command exit status"
test_case "false" "False command"
test_case "echo \$?" "False command exit status"
test_case "ls > /dev/null" "Successful ls"
test_case "echo \$?" "Successful ls exit status"
test_case "ls nonexistent 2>/dev/null" "Failed ls"
test_case "echo \$?" "Failed ls exit status"
test_case "/bin/true" "Absolute path true"
test_case "echo \$?" "Absolute path true exit status"
test_case "/bin/false" "Absolute path false"
test_case "echo \$?" "Absolute path false exit status"
test_case "echo hello" "Echo command"
test_case "echo \$?" "Echo exit status"
test_case "nonexistent_cmd 2>/dev/null" "Command not found"
test_case "echo \$?" "Command not found exit status"

echo -e "${BLUE}=== INPUT REDIRECTION (<) ===${NC}"

test_case "cat < $TMP_FILE" "Basic input redirection"
test_case "wc -l < $TMP_FILE" "Input redirection with wc"
test_case "cat < nonexistent_file" "Input redirection nonexistent file"
test_case "sort < $TMP_FILE" "Input redirection with sort"

echo -e "${BLUE}=== OUTPUT REDIRECTION (>) ===${NC}"

test_case "echo hello > output.txt" "Basic output redirection"
test_case "cat output.txt" "Check redirected output"
test_case "rm output.txt" "Cleanup output file"
test_case "ls > files.txt" "Output redirection with ls"
test_case "head -3 files.txt" "Check ls output"
test_case "rm files.txt" "Cleanup files list"
test_case "echo hello > /dev/null" "Output to /dev/null"
test_case "pwd > current_dir.txt" "pwd output redirection"
test_case "cat current_dir.txt" "Check pwd output"
test_case "rm current_dir.txt" "Cleanup pwd file"

echo -e "${BLUE}=== APPEND REDIRECTION (>>) ===${NC}"

test_case "echo line1 > append.txt" "Create file for append"
test_case "echo line2 >> append.txt" "Append to file"
test_case "cat append.txt" "Check append result"
test_case "rm append.txt" "Cleanup append file"
test_case "echo hello >> new_file.txt" "Append to new file"
test_case "cat new_file.txt" "Check new append file"
test_case "rm new_file.txt" "Cleanup new file"
test_case "ls >> output.txt" "Append ls output"
test_case "echo '---' >> output.txt" "Append separator"
test_case "cat output.txt" "Check multiple appends"
test_case "rm output.txt" "Cleanup append test"

echo -e "${BLUE}=== HERE DOCUMENT (<<) ===${NC}"

test_case "cat << EOF
hello
world
EOF" "Basic here document"

test_case "wc -l << END
line1
line2
line3
END" "Here document with wc"

test_case "cat << DELIMITER
testing here doc
with multiple lines
DELIMITER" "Here document custom delimiter"

test_case "grep hello << EOF
hello world
goodbye world
hello again
EOF" "Here document with grep"

echo -e "${BLUE}=== PIPES (|) ===${NC}"

test_case "echo hello | cat" "Basic pipe"
test_case "ls | wc -l" "Pipe to word count"
test_case "echo -e 'c\na\nb' | sort" "Pipe with sort"
test_case "ls | grep minishell" "Pipe with grep"
test_case "cat $TMP_FILE | wc -w" "Pipe with file input"
test_case "echo hello | cat | cat" "Multiple pipes"
test_case "ls -l | head -5 | tail -2" "Complex pipe chain"
test_case "echo test | cat | wc -c" "Three command pipeline"
test_case "env | grep USER | head -1" "Environment pipe chain"

echo -e "${BLUE}=== COMPLEX COMBINATIONS ===${NC}"

test_case "cat < $TMP_FILE | wc -l" "Input redirection with pipe"

echo -e "${BLUE}=== SYNTAX ERRORS (SHOULD BE HANDLED) ===${NC}"

test_case "echo hello >" "Redirection without target"
test_case "echo < " "Input redirection without source"
test_case "| echo hello" "Pipe at beginning"
test_case "echo hello |" "Pipe at end"
test_case "echo hello | | cat" "Double pipe"
test_case "cat <<" "Here document without delimiter"
test_case ">" "Lone redirection"
test_case "<" "Lone input redirection"

echo -e "${BLUE}=== UNCLOSED QUOTES (SHOULD NOT BE INTERPRETED) ===${NC}"

test_case "echo 'unclosed quote" "Unclosed single quote"
test_case 'echo "unclosed quote' "Unclosed double quote"
test_case "echo 'nested \"quote" "Nested unclosed quotes"

echo -e "${BLUE}=== EDGE CASES ===${NC}"

test_case "" "Empty input"
test_case "   " "Whitespace only"
test_case "echo    hello    world" "Multiple spaces"
test_case "echo hello; echo world" "Commands with semicolon (should not work per subject)"
test_case "echo hello && echo world" "AND operator (should not work per subject)"
test_case "echo hello || echo world" "OR operator (should not work per subject)"

echo -e "${BLUE}=== SIGNAL BEHAVIOR TESTS ===${NC}"
echo "Note: These test signal handling - Ctrl-C, Ctrl-D, Ctrl-\\"
echo "Manual testing required for interactive behavior"

# Test basic command execution for signal context
test_case "echo testing signal context" "Basic command for signal testing"

# Cleanup
rm -f "$TMP_OUT" "$TMP_OUT.clean" "$TMP_REF" "$TMP_FILE" output.txt append.txt files.txt current_dir.txt new_file.txt temp.txt lines.txt

# Summary
echo -e "\n${BLUE}=== TEST SUMMARY ===${NC}" | tee -a "$LOG"
echo "Total tests: $TOTAL_TESTS" | tee -a "$LOG"
echo -e "${GREEN}Passed: $PASSED_TESTS${NC}" | tee -a "$LOG"
echo -e "${RED}Failed: $FAILED_TESTS${NC}" | tee -a "$LOG"

if [ $FAILED_TESTS -eq 0 ]; then
    echo -e "${GREEN}🎉 ALL MANDATORY TESTS PASSED! 🎉${NC}" | tee -a "$LOG"
    exit 0
else
    echo -e "${RED}❌ Some tests failed. Check $LOG for details.${NC}" | tee -a "$LOG"
    echo -e "${YELLOW}Remember: Take bash as reference for any unclear behavior${NC}"
    exit 1
fi

echo -e "\n${BLUE}=== MANUAL TESTING REMINDERS ===${NC}"
echo "Don't forget to manually test:"
echo "1. Interactive prompt display"
echo "2. Command history (up/down arrows)"
echo "3. Ctrl-C behavior (new prompt on new line)"
echo "4. Ctrl-D behavior (exit shell)"
echo "5. Ctrl-\\ behavior (do nothing)"
echo "6. Global variable usage (only for signal storage)"
