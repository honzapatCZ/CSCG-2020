# CSCG

Hi! This is my documentation(and kinda write up) of solving CSCG-2020 game hacking challenges.

## Follow White Rabbit

### 1st
Quick look at folder structure shows its unity. _Data/Managed/Assembly-CSharp.dll so DnSpy to the rescue.
Yes its not obduscated. I open the game. Follow the first rabbit. Jump down and oh. I died.
So dnspy, ok theres PlayerController, and Die(), lets see who it uses. Ok deleted from Controller_Hit.
And yes, first flag.
1st challenge done.
### 2nd
So you say there is supposed to be house under construction(I got over the wall finding a spot where the wall is small enough). OK lets see where I can OnGUI(old IMGUI Unity rendering method) something. You know what why not use PlayerController. OK I got Button. OK I got All Levels in game listed. Hmm a FlagLand_Update.
Now where I can get some action. So you say something about UI. And there is this if with multiple Input.GetKeyDown(). Hmm lets try it. OK So I didnt need to edit I could have just pressed FLAG all the time, whatever, now to use it. So change it to SceneManager.LoadScene("Flagland_Update", LoadMode.Additive) and boom, there is the update. Ah shit. I dont want to do that parkour. Lets change the Jump behaviour, lets say add 50. And ok Iam up. now just how to get thru the bars. So just somewhere to check for Input.getkeydown(lets say n) and set the player position 1 forward. AAAAND iam there. Now the hardest part to actually understand whats that combination of text and image. After some assumption flag submitted.

## Maze

So first thing, I go to the directory, automatically to _Data, and wait... there is no Managed folder, just some il2cpp bullshit. Quick google search reveals that its Unitys upcoming compilation method(slowly being incorporated as main one for distributon), so easy modding is over guys.
So I open the game, login, ok its an online game, so maybe some proxy as liveoverflow used in PwnAdventure, I download it, change ports, and it .... doesnt work. I notice that PawnAdventure is using TCP and Maze is using UDP, thats bad, I spend rest of the day rewriting the code to work with UDP.
After a while I load it up, forward maze.liveoverflow.com to 127.0.0.1 and boom:
```
400f0c6008575bc38dd1ddc1be849a77507831404f5e6d7c8b9aa9b8c7d6e5f404132231404f5e6d7c8b9aa9
7fac431ff7d308f9c32f9cf44d669544f09d4af6
fa8eb3a64e9c128b45980aca9fe456
3ab773deff5b3e989f38925b651fd6
1f194f170750a4d4a2b6827296314c657e97b0d8e7d7150e09607935d8e2ddf6102942bb46a4a6bfd8f10b263d569777
190c25161e6b7372297a01e0da09abb5c1cdd9e5
31c461daec456341daea325774a76a2ff3b87d5302e790545cdea3cf5ed7b67b4005c96e6131dca1662befb4793eab38
c489f87df837d053b59069ff133135c04ad35de6
7d41418d2f17b8e44d51ffadb6238fcd0f5091d2
fcd1ace1f648635ef0c1eeb14002d2a476481adbb8a3616607d6a8d33f38efc1936537e9e8857e5022f3c597693b0dde
e1b8dda97c5afe5a7ef9df0530a28943fbb46d26
```
Quick DM to liveoverflow shows that he wont give me any hint.

So I google a bit about that IL2Cpp, and stumble upon a IL2CPP Inspector, hmm not much of a use and IL2CPP Dumper, ya thats what I like tailored for unity. I open it up give it all it needs and boom, it generated some files.
Dummy.cs - not much use
Ida - I dont have it
 **So I give up**

After some time i stumble upon liveoverflow video with GuidedHacking, about hacking games with dll.
So cheat engine it is.
After 30 ptrscans I found the position pointer, so I can start overriding it. Done.
So wait I still cant move. Quick CE about what writes to that address and few more lines of code.
And yeeees I can fly up, so I completed lava. Tower was pretty similiar, but just too lengthy.
