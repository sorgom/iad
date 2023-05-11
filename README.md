# iad
internet access device

# preconditions
## python 3
## linked libraries
### sqlite3

```shell
sudo apt-get install sqlite3 libsqlite3-dev
```

### zlib
```shell
sudo apt-get install zlib1g
```

## cmake

# directory structures
## project
### srv
contains iad server build sources
### templates
templates for runtime installation

## target
default target folder: $HOME/iad
subfolders:
### bin
iadsrv binary and ini file
### db
sqlite3 databases
### htm
sample presentations
### bbl
translation files
### sh
runtime shell scripts
