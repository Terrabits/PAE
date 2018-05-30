# PAE Timing info

## Alternating Sweeps

## Pulse Generator

These commands set up the pulse signal on the ZVA. The ZNB does not have an internal pulse generator; an external signal must be used.

| Setting             | SCPI                     | Parameters                    | Requirement                                          |
| ------------------- | ------------------------ | ----------------------------- | ---------------------------------------------------- |
| State               | `SENS<Ch>:PULS:GEN`      | `ON`, `OFF`                   | `ON`                                                 |
| Pulse type          | `SENS<Ch>PULS:GEN1:TYPE` | `SING`, `TRA`, `CHIG`, `CLOW` | `SING`                                               |
| Pulse width         | `SENS<Ch>PULS:GEN1:WIDT` | width, in s                   | greater than integration time of DMM                 |
| Pulse period        | `SENS<Ch>PULS:GEN1:PER`  | period, in s                  | greater than integration time + recovery time of DMM |
| Delay               | `SENS<Ch>:PULS:GEN1:PER` | delay, in s                   | Don't care                                           |
| Polarity            | `SENS<Ch>:PULS:GEN1:POL` | `NORM`, `INV`                 | Don't care                                           |
| Apply to channel(s) | `SENS<Ch>PULS:GEN1:MODE` | `CSP`, `CONT`                 | `CSP` (single channel)                               |

Note: `GEN1` is the pulse generator, `GEN2` is sync. Most of the commands work for both with the appropriate `GEN` suffix.

## Trigger Settings

| Setting  | SCPI            | Parameters                                    | Requirement     |
| -------- | --------------- | --------------------------------------------- | --------------- |
| Source   | `TRIG<Ch>:SOUR` | `IMM`, `EXT`, `TIM`, `MAN`, `PGEN`            | `PGEN` or `EXT` |
| Slope    | `TRIG<Ch>:SLOP` | `POS`, `NEG`                                  | Don't care      |
| Sequence | `TRIG<Ch>:LINK` | `'POINT'`, `'SWEEP'`, `'PPOINT'`, `'SEGMENT'` | `'PPOIN'`       |
| Delay    | `TRIG<Ch>:HOLD` | delay, in s                                   | `0`             |

## User Control: Busy Signal

| Setting                           | SCPI                      | Parameters                          | Requirement |
| --------------------------------- | ------------------------- | ----------------------------------- | ----------- |
| Busy signal (User Control, pin 4) | `OUTP<Ch>:UPOR:BUSY:LINK` | `AUTO`, `POI`, `PPO`, `SEGM`, `SWE` | `PPO`       |
