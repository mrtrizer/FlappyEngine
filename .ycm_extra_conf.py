def FlagsForFile( filename, **kwargs ):
  return {
    'flags': [ '-std=c++11', '-x', 'c++', '-Wall', '-Wextra', '-Werror' ],
  }
