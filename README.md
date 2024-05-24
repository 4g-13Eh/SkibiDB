# SkibiDB

A windows-only, secure, lightweight, and fast Database Management System written in C++

<p align="center">
  <img width="460" height="300" src="https://static.wikia.nocookie.net/fcoc-vs-battles/images/f/fc/Skibidi-toilet.gif/revision/latest/scale-to-width-down/165?cb=20240401150600" />
</p>

## Libraries

- [nlohmann/json](https://github.com/nlohmann/json)
- [ikalnytskyi/termcolor](https://github.com/ikalnytskyi/termcolor)

## Installation

```bash
$ git clone https://github.com/FujiwaraChoki/SkibiDB.git --recursive
$ cd SkibiDB
$ mkdir build
$ cd build
$ cmake ..
$ make
```

## Usage

```bash
$ ./skibidb
```

To see a list of possible commands, type "HELP".

# Examples

### Create a `users` table

`CREATE TABLE users`

`SAVE`

### Add a user

`INSERT INTO users (username, age) VALUES ("FujiwaraChoki", 17)`

`SAVE`

### Select specific user

`SELECT username FROM users WHERE username ~ "s/FujiwaraShoki"`

The `s/` here is a flag you can add if you'd like to use the soundex search. More information [here](https://en.wikipedia.org/wiki/Soundex).

`~` is the same as `LIKE`, it doesn't matter which one you provide.

### Delete a user

`DELETE FROM users WHERE username LIKE "s/Fujiwarashoki"`

This commands deletes every user, who's username sounds like "Fujiwarashoki".

`SAVE`

There's also the basic `DROP`, `UPDATE` and `EXIT` commands.

## Set a Primary Key

`SET PK users user_id`

`SAVE`

# Functions

The only supported function as of now, is `COUNT`.

## License

[MIT](LICENSE)

## Authors

- [FujiwaraChoki](https://github.com/FujiwaraChoki)
- [4g-13Eh](https://github.com/4g-13Eh)
- [FischerNils06](https://github.com/FischerNils06)

