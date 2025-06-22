// Your existing global signal variable
extern int g_sig;

// Simple signal handler for heredoc - only sets the flag


char *heredoc(t_cmd *cmd, t_mem *gc, int *index)
{
    char        *clean_delim;
    char        filepath[64];
    int         fd;
    char        *line;
    char        *filepath_copy;
    void        (*old_handler)(int);

    if (!cmd || !cmd->delimiter)
        return (NULL);

    clean_delim = remove_quotes(gc, cmd->delimiter);
    if (!clean_delim)
        return (NULL);

    generate_filename(filepath, sizeof(filepath), (*index)++);
    fd = create_heredoc_file(filepath);
    if (fd < 0)
        return (NULL);

    // Install heredoc-specific signal handler
    old_handler = signal(SIGINT, handle_heredoc_sigint);
    g_sig = 0; // Reset signal flag

    while (1)
    {
        write(STDOUT_FILENO, "> ", 2);
        line = get_next_line(STDIN_FILENO);

        // Check for signal immediately after get_next_line
        if (g_sig)
        {
            if (line)
                free(line);
            close(fd);
            unlink(filepath);
            signal(SIGINT, old_handler); // Restore old handler
            return (NULL);
        }

        if (!line) // EOF (Ctrl+D)
        {
            ft_perror("\nminishell: warning: here-document delimited by end-of-file\n");
            break;
        }

        if (compare_delimiter(line, clean_delim))
        {
            free(line);
            break;
        }

        write(fd, line, ft_strlen(line));
        free(line);

        // Check for signal after processing line
        if (g_sig)
        {
            close(fd);
            unlink(filepath);
            signal(SIGINT, old_handler);
            return (NULL);
        }
    }

    // Restore original signal handler
    close(fd);

    filepath_copy = our_strdup(gc, filepath);
    return (filepath_copy);
}

// Fork-based approach (recommended if fork/waitpid are allowed)
char *heredoc_fork(t_cmd *cmd, t_mem *gc, int *index)
{
    char        *clean_delim;
    char        filepath[64];
    int         fd;
    char        *line;
    char        *filepath_copy;
    pid_t       pid;
    int         status;

    if (!cmd || !cmd->delimiter)
        return (NULL);

    clean_delim = remove_quotes(gc, cmd->delimiter);
    if (!clean_delim)
        return (NULL);

    generate_filename(filepath, sizeof(filepath), (*index)++);
    fd = create_heredoc_file(filepath);
    if (fd < 0)
        return (NULL);

    pid = fork();
    if (pid == -1)
    {
        close(fd);
        unlink(filepath);
        return (NULL);
    }

    if (pid == 0) // Child process
    {
        // Child gets default signal behavior
        signal(SIGINT, SIG_DFL);

        while (1)
        {
            write(STDOUT_FILENO, "> ", 2);
            line = get_next_line(STDIN_FILENO);

            if (!line) // EOF
            {
                ft_perror("\nminishell: warning: here-document delimited by end-of-file\n");
                break;
            }

            if (compare_delimiter(line, clean_delim))
            {
                free(line);
                break;
            }

            write(fd, line, ft_strlen(line));
            free(line);
        }

        close(fd);
        exit(0); // Child exits normally
    }
    else // Parent process
    {
        close(fd); // Parent doesn't need the fd

        // Parent ignores SIGINT during heredoc
        signal(SIGINT, SIG_IGN);
        waitpid(pid, &status, 0);

        // Restore original signal handler
        signal(SIGINT, handle_sigint);

        // Check if child was killed by SIGINT
        if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
        {
            unlink(filepath);
            g_sig = 1; // Set our global flag
            return (NULL);
        }

        // Check for other errors
        if (!WIFEXITED(status) || WEXITSTATUS(status) != 0)
        {
            unlink(filepath);
            return (NULL);
        }

        filepath_copy = our_strdup(gc, filepath);
        return (filepath_copy);
    }
}

// Update your main loop to handle heredoc interruption
int main(int ac, char **av, char **env)
{
    t_env   *g_env;
    t_mem   gc;
    t_cmd   *cmd;
    char    *line;
    int     g_exit;
    int     input;

    if (ac != 1)
        return ((void)av, ft_perror("Invalid number of arguments\n"));

    g_env = init_shell(env, &gc);
    g_exit = 0;

    while (1)
    {
        g_sig = 0;
        signal(SIGINT, handle_sigint);
        signal(SIGQUIT, SIG_IGN);

        input = get_input(&line, &gc);
        if (input == 0)
            break;
        if (input == 1)
            continue;

        signal(SIGINT, SIG_IGN);
        cmd = parse_input(line, g_env, &gc);
        free(line);

        if (cmd)
        {
            g_exit = process_command(cmd);

            // If heredoc was interrupted, set appropriate exit status
            if (g_sig)
                g_exit = 130;
        }
        else
            g_exit = ft_perror("Parse error.\n");
    }

    ft_free_all(&gc);
    exit(g_exit);
}
