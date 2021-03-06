/**
 * @file
 *
 * @brief Macros by Elektra.
 *
 * Macros start with ELEKTRA_ and are uppercase.
 *
 * @copyright BSD License (see LICENSE.md or https://www.libelektra.org)
 */


#ifndef KDBMACROS_H
#define KDBMACROS_H

#define ELEKTRA_QUOTE(x) #x

#if defined(__APPLE__)
#define ELEKTRA_STAT_SECONDS(status) status.st_mtime
#define ELEKTRA_STAT_NANO_SECONDS(status) status.st_mtimespec.tv_nsec
#elif defined(_WIN32)
#define ELEKTRA_STAT_SECONDS(status) status.st_mtime
#define ELEKTRA_STAT_NANO_SECONDS(status) 0
#else
#define ELEKTRA_STAT_SECONDS(status) status.st_mtim.tv_sec
#define ELEKTRA_STAT_NANO_SECONDS(status) status.st_mtim.tv_nsec
#endif

#define ELEKTRA_SET_ERROR_GET(parentKey)                                                                                                   \
	do                                                                                                                                 \
	{                                                                                                                                  \
		if (errno == EACCES)                                                                                                       \
			ELEKTRA_SET_ERROR (109, parentKey, strerror (errno));                                                              \
		else                                                                                                                       \
			ELEKTRA_SET_ERROR (110, parentKey, strerror (errno));                                                              \
	} while (0)

#define ELEKTRA_SET_ERROR_SET(parentKey)                                                                                                   \
	do                                                                                                                                 \
	{                                                                                                                                  \
		if (errno == EACCES)                                                                                                       \
			ELEKTRA_SET_ERROR (9, parentKey, strerror (errno));                                                                \
		else                                                                                                                       \
			ELEKTRA_SET_ERROR (75, parentKey, strerror (errno));                                                               \
	} while (0)

#define ELEKTRA_MALLOC_ERROR(key, size) ELEKTRA_SET_ERRORF (ELEKTRA_ERROR_MALLOC, key, "Unable to allocate %zu bytes.", size);

/**
 * @brief Sets error 84 if info != returned
 *
 * @param info how the info is now (freshly received)
 * @param returned how the info passed from user is
 * @param error key to set error to
 *
 * @return with -1 on error
 */
#define ELEKTRA_SET_ERROR_READ_ONLY(info, returned, error)                                                                                 \
	do                                                                                                                                 \
	{                                                                                                                                  \
		Key * k;                                                                                                                   \
		ksRewind (info);                                                                                                           \
		ksRewind (returned);                                                                                                       \
		while ((k = ksNext (returned)))                                                                                            \
		{                                                                                                                          \
			Key * c = ksNext (info);                                                                                           \
			if (!c)                                                                                                            \
			{                                                                                                                  \
				ELEKTRA_SET_ERRORF (84, error, "the key %s (value %s) was added", keyName (k), keyString (k));             \
				ksDel (info);                                                                                              \
				return -1;                                                                                                 \
			}                                                                                                                  \
			if (strcmp (keyName (k), keyName (c)) || strcmp (keyString (k), keyString (c)))                                    \
			{                                                                                                                  \
				ELEKTRA_SET_ERRORF (84, error, "the key %s (expected %s) was modified to %s (expected %s)", keyName (k),   \
						    keyName (c), keyString (k), keyString (c));                                            \
				ksDel (info);                                                                                              \
				return -1;                                                                                                 \
			}                                                                                                                  \
		}                                                                                                                          \
		if ((k = ksNext (info)) != 0)                                                                                              \
		{                                                                                                                          \
			ELEKTRA_SET_ERRORF (84, error, "the key %s (value %s) was removed", keyName (k), keyString (k));                   \
			ksDel (info);                                                                                                      \
			return -1;                                                                                                         \
		}                                                                                                                          \
		ksDel (info);                                                                                                              \
	} while (0)


#endif
