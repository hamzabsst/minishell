

// Updated make_env_var to use memory manager
static char	*make_env_var(char *key, char *value, t_mem *manager)
{
	char	*var;
	size_t	len;

	len = ft_strlen(key) + ft_strlen(value) + 2;
	var = ft_malloc(manager, len);
	if (!var)
		return (NULL);
	ft_strcpy(var, key);
	ft_strcat(var, "=");
	ft_strcat(var, value);
	return (var);
}

// Updated update_env to use memory manager
int	update_env(char ***env_ptr, char *key, char *value, t_mem *manager)
{
	char	**env;
	char	*new_var;
	size_t	key_len;
	int		i;

	if (!env_ptr || !*env_ptr || !key || !value)
		return (1);
	env = *env_ptr;
	key_len = ft_strlen(key);
	new_var = make_env_var(key, value, manager);
	if (!new_var)
		return (1);
	i = 0;
	while (env[i])
	{
		if (!ft_strncmp(env[i], key, key_len) && env[i][key_len] == '=')
		{
			// Don't free here - let memory manager handle it
			env[i] = new_var;
			return (0);
		}
		i++;
	}
	return (copy_env(env_ptr, new_var, i, manager));
}
