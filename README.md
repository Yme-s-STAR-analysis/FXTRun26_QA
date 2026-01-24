# FXT Run 2026 QA - Fastoffline

`version 1.3`

`author: yghuang`

## Usage

1. Modify settings (trigger sets and run list) in `conf.py` and run `python3 MakeDbConf.py`.

2. Using `./prepare.sh PATH FILELIST 1` to initialize and submit jobs.

    * replace `1` with `0`: only prepare the folder but do not submit jobs.

3. Using `./postrun.sh fmarge` to merge the outputs.

4. Run `MakePlots.cxx` and / or `MakeCmpPlots.cxx` to save pdf and / or png files.

5. Run `BetterProfile.cxx` to make those profiles better formatted for official run-by-run QA package.

## Change log

2026 Jan. 25 by yghuang 1.3:

> Add a new macro, to cancel Profile entry with bin content exactly equals to 0.

> Add a new macro, to make overlapped acceptance plot.

> Add a new macro, to make statistic plot.

> Update X (and Y) axis range for some of the histograms. 

2026 Jan. 23 by yghuang 1.2:

> Updated run-by-run QA TProfiles, and some more histograms.

2026 Jan. 22 by yghuang (2.4 -> 1.0):

> Dedicated version of FXT run2026 fastoffline data.
