# FXT Run 2026 QA - Fastoffline

`version 1.2`

`author: yghuang`

## Usage

1. Modify settings (trigger sets and run list) in `conf.py` and run `python3 MakeDbConf.py`.

2. Using `./prepare.sh PATH FILELIST 1` to initialize and submit jobs.

    * replace `1` with `0`: only prepare the folder but do not submit jobs.

3. Will create the reduced qa histograms, and all the qa processes can be done based on this. 

4. Run `MakePlots.cxx` and / or `MakeCmpPlots.cxx` to save pdf and / or png files.

## Change log

2026 Jan. 23 by yghuang 1.2:

> Updated run-by-run QA TProfiles, and some more histograms.

2026 Jan. 22 by yghuang (2.4 -> 1.0):

> Dedicated version of FXT run2026 fastoffline data.
