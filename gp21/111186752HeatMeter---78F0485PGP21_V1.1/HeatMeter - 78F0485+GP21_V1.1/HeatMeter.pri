[Project.ID]
Ver=200
Target=IDK032G
[Configuration]
Chip=uPD78F0485
Internal Rom=60KB
Internal Ram=1024B
Internal Bank Rom=0KB
Internal Extend Ram=1024B
Clock=Clock Board
Sub Clock=
Peripheral Break=OFF
Firm Clock=System
Target Power Off=Not Permit
Fail Safe Break=0x2000
NMI=OFF
TRESET=OFF
IRESET=OFF
WAIT=OFF
SysClock=4.00
SubClock=
[Mapping]
Count=0
[Main]
Geometry=22, 29, 960, 551
Window=Normal
MDI_MAX=OFF
Button=ON
Mode=Auto
Trace=Uncond ON
Trace2=Non Stop
Coverage=OFF
Timer=OFF
Tracer=OFF
[Load File]
Dir=.
SaveFilter=0
Start=0
End=0
LoadFilter=5
Offset=0
Object=ON
Symbol=ON
Erase=ON
Symbol Reset=ON
CPU Reset=ON
File1=a.lmf
LoadFilter1=5
Bank Format1=OFF
Offset1=0
Object1=ON
Symbol1=ON
Erase1=ON
CPU Reset1=ON
Symbol Reset1=ON
[View File]
Dir=.
Filter=Source
[Debugger Option]
Source Path="Source"
Symbol Size=Byte
Symbol Format=Hex
Register Name=Func
Offset Label=OFF
Offset Mnemonic=ON
Tab Count=4
Tab Size1=*.*, 8
Tab Size2=*.c, 8
Tab Size3=*.s, 8
Tab Size4=*.asm, 8
Default Source=*.c;*.s;*.asm
Default Module=*.lnk;*.lmf
Startup Start=_@cstart
Startup End=_@cend
Main Symbol=_main
Symbol Type=OFF
Language=C
Kanji=SJIS
[Source]
Geometry=0, 0, 600, 400
Window=Normal
DispStart=61
CaretPos=62,0
Mode=Normal
DispFile=
Address1=
Address2=
Address3=
Address4=
Address5=
Address6=
Address7=
Address8=
Address9=
Address10=
Address11=
Address12=
Address13=
Address14=
Address15=
Address16=
Data1=
Data2=
Data3=
Data4=
Data5=
Data6=
Data7=
Data8=
Data9=
Data10=
Data11=
Data12=
Data13=
Data14=
Data15=
Data16=
Option=0
Case=ON
Direction=Down
File1=
File2=
File3=
File4=
File5=
File6=
File7=
File8=
File9=
File10=
File11=
File12=
File13=
File14=
File15=
File16=
SaveRange=Screen
SaveStart=
SaveEnd=
Accumulative=ON
[Assemble]
Geometry=0, 0, 0, 0
Window=Hide
DispStart=847473664
CaretPos=0,0
Address1=
Address2=
Address3=
Address4=
Address5=
Address6=
Address7=
Address8=
Address9=
Address10=
Address11=
Address12=
Address13=
Address14=
Address15=
Address16=
Data1=
Data2=
Data3=
Data4=
Data5=
Data6=
Data7=
Data8=
Data9=
Data10=
Data11=
Data12=
Data13=
Data14=
Data15=
Data16=
Case=ON
Scan=OFF
Direction=Down
FindStart=
FindEnd=
SaveRange=Screen
SaveStart=
SaveEnd=
[Memory]
Geometry=0, 0, 0, 0
Window=Hide
Boundary=0
Format=Hex
Mode=Byte
Endian=
Ascii=OFF
Address=
DispStart=FFFFFFFF
CaretPosData=0, 0
CaretPosAscii=0, 0
Address1=
Address2=
Address3=
Address4=
Address5=
Address6=
Address7=
Address8=
Address9=
Address10=
Address11=
Address12=
Address13=
Address14=
Address15=
Address16=
Data1=
Data2=
Data3=
Data4=
Data5=
Data6=
Data7=
Data8=
Data9=
Data10=
Data11=
Data12=
Data13=
Data14=
Data15=
Data16=
Binary=ON
Scan=OFF
Direction=DOWN
FindStart=
FindEnd=
Unit=Byte
SaveRange=Screen
SaveStart=
SaveEnd=
Accumulative=ON
[Memory fill]
Scope=0, 0
Code=0
[Memory copy]
Scope=0, 0
To=0
[Memory compare]
Source=0, 0
Destination=0
[I/O Port]
Line=0
[Stack]
Geometry=0, 0, 0, 0
Window=Hide
Boundary=0
Mode=Proper
[Sfr]
Geometry=0, 0, 0, 0
Window=Hide
Address1=
Address2=
Address3=
Address4=
Address5=
Address6=
Address7=
Address8=
Address9=
Address10=
Address11=
Address12=
Address13=
Address14=
Address15=
Address16=
Boundary=0, 0
Mode=Hex
Attribute=Show
Sort=Address
Pickup=OFF
SelectSort=Address
Last Name=
Line=365
L1=RXB6
L2=P1
L3=P2
L4=P3
L5=P4
L6=TXB6
L7=ADCR
L8=ADCRH
L9=P8
L10=P9
L11=P10
L12=P11
L13=P12
L14=P13
L15=P14
L16=P15
L17=TM00
L18=CR000
L19=CR010
L20=TM50
L21=CR50
L22=CMP00
L23=CMP10
L24=CMP01
L25=CMP11
L26=SIO10
L27=PF1
L28=PM1
L29=PM2
L30=PM3
L31=PM4
L32=PM8
L33=PM9
L34=PM10
L35=PM11
L36=PM12
L37=PM13
L38=PM14
L39=PM15
L40=HIOTRM
L41=PU1
L42=PU3
L43=PU4
L44=PU8
L45=PU9
L46=PU10
L47=PU11
L48=PU12
L49=PU13
L50=PU14
L51=PU15
L52=CKS
L53=CLOE
L54=BZOE
L55=CR51
L56=TMHMD2
L57=TOEN2
L58=TOLEV2
L59=TMHE2
L60=TMC51
L61=TOE51
L62=LVR51
L63=LVS51
L64=TCE51
L65=CMP02
L66=CMP12
L67=MC0STR
L68=MC0TSF
L69=EGP
L70=EGN
L71=MC0TX
L72=MC0BIT
L73=MC0CTL0
L74=MC0OLV
L75=MC0OSL
L76=MC0DIR
L77=MC0PWR
L78=MC0CTL1
L79=MC0CTL2
L80=ISC
L81=ISC2
L82=ISC3
L83=ASIM6
L84=RXE6
L85=TXE6
L86=POWER6
L87=TM52
L88=ASIS6
L89=RTCCL
L90=ASIF6
L91=CKSR6
L92=BRGC6
L93=ASICL6
L94=SBRT6
L95=SBRF6
L96=CR52
L97=TCL52
L98=TMC52
L99=TCE52
L100=RSUBC
L101=SEC
L102=MIN
L103=HOUR
L104=WEEK
L105=DAY
L106=MONTH
L107=YEAR
L108=TMHMD0
L109=TOEN0
L110=TOLEV0
L111=TMHE0
L112=TCL50
L113=TMC50
L114=TOE50
L115=LVR50
L116=LVS50
L117=TCE50
L118=TMHMD1
L119=TOEN1
L120=TOLEV1
L121=TMHE1
L122=TMCYC1
L123=NRZ1
L124=KRM
L125=TM51
L126=ASIM0
L127=RXE0
L128=TXE0
L129=POWER0
L130=BRGC0
L131=RXB0
L132=ASIS0
L133=TXS0
L134=CSIM10
L135=CSIE10
L136=CSIC10
L137=SUBCUD
L138=SOTB10
L139=ALARMWM
L140=ALARMWH
L141=ALARMWW
L142=RTCC0
L143=RCLOE0
L144=RCLOE1
L145=RTCE
L146=RTCC1
L147=RWAIT
L148=RWST
L149=RIFG
L150=WAFG
L151=WALIE
L152=WALE
L153=RTCC2
L154=RCKDIV
L155=RCLOE2
L156=RINTE
L157=TCL51
L158=ADM
L159=ADCE
L160=ADCS
L161=ADS
L162=ADISS
L163=ADPC0
L164=CSIMA0
L165=RXEA0
L166=TXEA0
L167=CSIAE0
L168=CSIS0
L169=CSIT0
L170=ATSTA0
L171=ATSTP0
L172=BRGCA0
L173=ADTP0
L174=ADTI0
L175=SIOA0
L176=ADTC0
L177=WDTE
L178=RMCN
L179=RMEN
L180=RMDR
L181=RMSCR
L182=RMGPLS
L183=RMGPLL
L184=OSCCTL
L185=OSCSELS
L186=OSCSEL
L187=EXCLK
L188=RCM
L189=RSTOP
L190=LSRSTOP
L191=RSTS
L192=MCM
L193=MCM0
L194=MCS
L195=XSEL
L196=MOC
L197=MSTOP
L198=OSTC
L199=OSTS
L200=RMGPHS
L201=RMGPHL
L202=RMDLS
L203=RMDLL
L204=RMDH0S
L205=RMDH0L
L206=RMSR
L207=RESF
L208=RMDH1S
L209=RMDH1L
L210=RMER
L211=LCDMD
L212=KSON
L213=KSF
L214=LCDM
L215=VAON
L216=SCOC
L217=LCDON
L218=LCDC0
L219=PF2
L220=PFALL
L221=TMC00
L222=OVF00
L223=PRM00
L224=CRC00
L225=TOC00
L226=TOE00
L227=LVR00
L228=LVS00
L229=OSPE00
L230=OSPT00
L231=LVIM
L232=LVIF
L233=LVIMD
L234=LVISEL
L235=LVION
L236=LVIS
L237=PFCMD
L238=PFS
L239=FPRERR
L240=FLPMC
L241=FLSPM0
L242=FWEPR
L243=FWEDIS
L244=IF0
L245=IF0L
L246=LVIIF
L247=PIF0
L248=PIF1
L249=PIF2
L250=PIF3
L251=PIF4
L252=PIF5
L253=SREIF6
L254=IF0H
L255=SRIF6
L256=STIF6
L257=CSIIF10
L258=STIF0
L259=TMIFH1
L260=TMIFH0
L261=TMIF50
L262=TMIF000
L263=TMIF010
L264=IF1
L265=IF1L
L266=ADIF
L267=SRIF0
L268=RTCIF
L269=TMIF51
L270=KRIF
L271=RTCIIF
L272=TMIF52
L273=IF1H
L274=TMHIF2
L275=MCGIF
L276=RINIF
L277=DFULLIF
L278=GPIF
L279=RENDIF
L280=RERRIF
L281=ACSIIF
L282=MK0
L283=MK0L
L284=LVIMK
L285=PMK0
L286=PMK1
L287=PMK2
L288=PMK3
L289=PMK4
L290=PMK5
L291=SREMK6
L292=MK0H
L293=SRMK6
L294=STMK6
L295=CSIMK10
L296=STMK0
L297=TMMKH1
L298=TMMKH0
L299=TMMK50
L300=TMMK000
L301=TMMK010
L302=MK1
L303=MK1L
L304=ADMK
L305=SRMK0
L306=RTCMK
L307=TMMK51
L308=KRMK
L309=RTCIMK
L310=TMMK52
L311=MK1H
L312=TMHMK2
L313=MCGMK
L314=RINMK
L315=DFULLMK
L316=GPMK
L317=RENDMK
L318=RERRMK
L319=ACSIMK
L320=PR0
L321=PR0L
L322=LVIPR
L323=PPR0
L324=PPR1
L325=PPR2
L326=PPR3
L327=PPR4
L328=PPR5
L329=SREPR6
L330=PR0H
L331=SRPR6
L332=STPR6
L333=CSIPR10
L334=STPR0
L335=TMPRH1
L336=TMPRH0
L337=TMPR50
L338=TMPR000
L339=TMPR010
L340=PR1
L341=PR1L
L342=ADPR
L343=SRPR0
L344=RTCPR
L345=TMPR51
L346=KRPR
L347=RTCIPR
L348=TMPR52
L349=PR1H
L350=TMHPR2
L351=MCGPR
L352=RINPR
L353=DFULLPR
L354=GPPR
L355=RENDPR
L356=RERRPR
L357=ACSIPR
L358=IMS
L359=IXS
L360=INTS
L361=INTC
L362=PCC
L363=CSS
L364=CLS
L365=R8MTRM
[Local Variable]
Geometry=0, 0, 0, 0
Window=Hide
Boundary=0
Mode=Proper
[Trace View]
Geometry=0, 0, 0, 0
Window=Hide
Mode=Normal
Frame=Show
Address=Show
Data=Hex
Status=Show
Address2=Show
Data2=Hex
Status2=Show
Disasm=Show
Pick Up=Off
Synchronize=0 0 0 0
Frameno=
Frameno1=
Frameno2=
Frameno3=
Frameno4=
Frameno5=
Frameno6=
Frameno7=
Frameno8=
Frameno9=
Frameno10=
Frameno11=
Frameno12=
Frameno13=
Frameno14=
Frameno15=
Frameno16=
ScanStatus=ALL
Access Size=B
ScanAddress=<>-<>
Mask Address=
ScanData=
Mask Data=
ScanExternal=
Mask External=
Scan=OFF
Direction=Down
FindStart=
FindEnd=
SaveRange=Screen
SaveStart=
SaveEnd=
[Register]
Geometry=0, 0, 0, 0
Window=Hide
Mode=Hex
Boundary=0
Pickup=OFF
Name=Functional
Register Bank=0
RP0=3
RP1=3
RP2=3
RP3=3
PC=3
SP=3
PSW=3
[Event Manager]
Geometry=0, 0, 0, 0
Window=Hide
Sort by=Unsort
Detail=OFF
[Event Set]
Geometry=0, 0, 0, 0
Window=Hide
Manager=ON
Sort by=Unsort
Detail=OFF
Last Name=
Count=0
[Event Link]
Geometry=0, 0, 0, 0
Window=Hide
Manager=ON
Sort by=Unsort
Detail=OFF
Last Name=
Count=0
[Break]
Geometry=0, 0, 0, 0
Window=Hide
Manager=ON
Sort by=Unsort
Detail=OFF
Last Name=
Count=0
[Trace]
Geometry=0, 0, 0, 0
Window=Hide
Manager=ON
Sort by=Unsort
Detail=OFF
Last Name=
Delay Count0=2
Count=0
[Snap]
Geometry=0, 0, 0, 0
Window=Hide
Manager=ON
Sort by=Unsort
Detail=OFF
Last Name=
Count=0
[Stub]
Geometry=0, 0, 0, 0
Window=Hide
Manager=ON
Sort by=Unsort
Detail=OFF
Last Name=
Count=0
[Timer]
Geometry=0, 0, 0, 0
Window=Hide
Manager=ON
Sort by=Unsort
Detail=OFF
Last Name=
TimerRateRunBreak=0014
TimerBreakOutRunBreak=OFF
Count=0
[Timer Monitor]
Count=0
[Dmm]
Geometry=0, 0, 0, 0
Window=Hide
Manager=ON
Sort by=Unsort
Detail=OFF
Last Name=
Count=0
[Variable]
Geometry=0, 0, 0, 0
Window=Hide
Boundary=0
Line=0
[Quick Watch]
0=
1=
2=
3=
4=
5=
6=
7=
8=
9=
10=
11=
12=
13=
14=
15=
[Software Break]
Geometry=0, 0, 0, 0
Window=Hide
Width=150 30 200 100
Count=0
[Reset]
Debugger=ON
Symbol=OFF
Target CPU=OFF
[Extended Option]
OnClick Software Break=ON
Redraw=500
Function=OFF
Break When Readout=Select
Use MINICUBE Extended Function=OFF
Verify=ON
Break Sound=ON
TraceAutoClear=ON
RegZeroClear=OFF
[About]
Version=Program
[RRM Setting]
Geometry=0, 0, 0, 0
Window=Hide
Num=0
Address1=0x0
Size1=0x0
Address2=0x0
Size2=0x0
Address3=0x0
Size3=0x0
Address4=0x0
Size4=0x0
Address5=0x0
Size5=0x0
Address6=0x0
Size6=0x0
Address7=0x0
Size7=0x0
Address8=0x0
Size8=0x0
[Pseudo Emulation]
Geometry=0, 0, 0, 0
Window=Hide
[Console]
Geometry=0, 0, 0, 0
Window=Hide