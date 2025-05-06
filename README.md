## Building

```bash
mkdir build 
cd build 
cmake ../
cmake --build .
```

## Usage

```bash
./app /path/to/the/file.txt
```

Some tests are located in tests/ folder.

## Issues
#### Task uncertainties 
1. It is unclear what to do with events, that overdue ending time.
2. Client can have a seat (id2) and then wait for a seat (id3). It results in absolute disaster when the person leaving (id4).
3. Even if a person didn't enter the building (id1), he still can wait for seats (id3).

#### Implementation specifics
- price is set up to uint64, as not specified exact constrains -> if price per hour is large, then owerflow might happen
- if client c sits and in that exact minute switches to another table([_, c] -> [c, _] all in XX:XX), then it would not cost him any money 
