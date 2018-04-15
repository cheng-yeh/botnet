# botnet
This project is an implementation of the essay, [**Botnet Detection Based on Anomaly and Community Detection**](https://ieeexplore.ieee.org/document/7422020/authors), 
 by *Jing Wang* and *Ioannis Ch. Paschalidis*.

I modify the last stage with [Combo](http://senseable.mit.edu/community_detection/), which may make the whole system more efficient without lost of performance.

## Usage
```
usage: ./botnet <--TotalList <fileName>> <--WindowNumber <window_number>>
                [--BotOne <bot1_IP,bot2_IP,...>] [--BotList <fileName>]
                [--OutputFile <outputFile>] [--Help] [--PivotTau <threshold>]
                [--ScgTau <threshold>]

mandatory arguments:
  --T <fileName>, --TotalList <fileName>
                Specify the original binetflow file.
  --WindowNumber <window_number>
                Provide the number of window. Must be a positive integer.

optional arguments:
  --BotOne <bot1_IP,bot2_IP,...>
                Ground-truth bots' IPs.
  --BotList <fileName>
                Specify the file consists of list of bot-related IPs.
  --OutputFile <outputFile>
                Provide the output directory.
  --Help
                Print help message.
  --PivotTau <threshold>
                Set the total interaction threshold for pivots. Must be a positive double.
  --ScgTau <threshold>
                Set the corelation coefficient threshold for SCG. Must be a positive double.
```
## Progress
The development of this project will be suspended until June.
