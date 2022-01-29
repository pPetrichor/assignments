# <font face="楷体"><center>计网作业7</center></font>

<p align="right"><font size=4 face="楷体">姓名：王瀚霖
学号：181860092
邮箱：603102048@qq.com</font></p>
------

### <font face="楷体">第8章</font>

<font size=4 face="楷体" color=#00bfff>R5. Consider an 8-block cipher. How many possible input blocks does this cipher have? How many possible mappings are there? If we view each mapping as a key, then how many possible keys does this cipher have? </font>

<font size=4 face="楷体">	答、有$2^8$种可能的输入块；有$2^8!$种可能的映射；视每个映射为一个密钥，则共有$2^8!$种可能的密钥。</font>



<font size=4 face="楷体" color=#00bfff>R6. Suppose N people want to communicate with each of N – 1 other people using symmetric key encryption. All communication between any two people, i and j, is visible to all other people in this group of N, and no other person in this group should be able to decode their communication. How many keys are required in the system as a whole? Now suppose that public key encryption is used. How many keys are required in this case? </font>

<font size=4 face="楷体">	答、利用对称密钥密码通信，要保证N个人中任意两个人有一个其他人都不知道的密钥；因此总共需要$\frac{N*(N-1)}{2}$个密钥；利用公开密钥密码的话，则每一个用户都会有一个公钥与一个私钥；每个人的公钥其他人都知道，所以给a发信息时，只要用a的公钥加密后发送，然后a用私钥解密即可；故只需要$2N$个密钥。</font>



<font size=4 face="楷体" color=#00bfff>R15. Suppose Alice has a message that she is ready to send to anyone who asks. Thousands of people want to obtain Alice’s message, but each wants to be sure of the integrity of the message. In this context, do you think a MAC-based or a digital-signature-based integrity scheme is more suitable? Why? </font>

<font size=4 face="楷体">	答、显然是基于数字签名好：如果基于MAC的话，Alice与这数以千计的每个人都要有一个共享的鉴别密钥；如果基于数字签名，则Alice只要用自己的私钥加密报文的散列，然后将签名的散列发送给这数以千计的人；每个人只要用Alice的公钥解密签名后与报文的散列比较即可验证报文的完整性，显然基于数字签名更好。</font>	



<font size=4 face="楷体" color=#00bfff>R23. Suppose Bob initiates a TCP connection to Trudy who is pretending to be Alice. During the handshake, Trudy sends Bob Alice’s certificate. In what step of the SSL handshake algorithm will Bob discover that he is not communicating with Alice? </font>

<font size=4 face="楷体">	答、如果Trudy向Bob发送了Alice的证书，则Bob会用Alice的公钥加密产生的PMS并发送给Trudy，但Trudy不知道Alice的私钥，所以无法解密得到Bob产生的PMS，也无法得到MS；所以在握手的最后阶段Trudy向Bob发送一个所有握手报文的MAC时，Bob会发现MAC鉴别失败，从而发现他没有在与Alice通信。</font>



<font size=4 face="楷体" color=#00bfff>P3. Consider the polyalphabetic system shown in Figure 8.4. Will a chosen plaintext attack that is able to get the plaintext encoding of the message “The quick brown fox jumps over the lazy dog.” be sufficient to decode all messages? Why or why not? </font>

<font size=4 face="楷体">	答、不足以；因为只有“The quick brown fox jumps over the lazy dog.”这一句与它的加密版本的话，虽然26个字母都出现在其中，但是在多码代替密码系统中不同位置的同一个字母可能加密结果不同。因此仅靠这一句是无法破解的。</font>



<font size=4 face="楷体" color=#00bfff>P7. (a) Using RSA, choose p = 3 and q = 11, and encode the word “dog” by encrypting each letter separately. Apply the decryption algorithm to the encrypted version to recover the original plaintext message. (b) Repeat part (a) but now encrypt “dog” as one message m. </font>

<font size=4 face="楷体">	答、</font>

<font size=4 face="楷体">	（a）、由题`p = 3, q = 11`，则`N = p * q = 33, z = (p - 1) * (q - 1) = 20`；取`e = 3`，e与`z = 20`互质，取`d = 7`作为e的模反元素：$(ed)\;mod\;z=(3*7)\;mod\;20=21\;mod\;20=1$；则公钥为(3,33)，私钥为(7,33)；按照a对应1、b对应2……规则来进行加密，则"dog"为"4，15，7"，加密的过程为：$c\;=\;m^e\;mod\;N$，即：</font>

| 字母(m) | 加密                    | 密文(c) |
| ------- | ----------------------- | ------- |
| d       | $4^3\;mod\;33$  =  $31$ | 31      |
| o       | $15^3\;mod\;33$  =  $9$ | 9       |
| g       | $7^3\;mod\;33$  =  $13$ | 13      |

<font size=4 face="楷体">	解密的过程为：$m\;=\;c^d\;mod\;N$，即：</font>

| 密文(c) | 解密                    | 明文 |
| ------- | ----------------------- | ---- |
| 31      | $31^7\;mod\;33$  =  $4$ | d    |
| 9       | $9^7\;mod\;33$  =  $15$ | o    |
| 13      | $13^7\;mod\;33$  =  $7$ | g    |

<font size=4 face="楷体">	（b）、按照5比特编码一个字母来解释，则"dog"的编码为：00100 01111 00111，对应于十进制m = 4583；选取`p = 37, q = 127 `，则`N = p * q = 4699, z = (p - 1) * (q - 1) = 4536 `；取`e = 13`，e与z互质，取`d = 349`作为e的模反元素：$(ed)\;mod\;z=(13*349)\;mod\;4536=4537\;mod\;4536=1$；则公钥为(13,4699)，私钥为(349,4699)；故对"dog"加密得：</font>
$$
m^e\;mod\;N=4583^{13}\;mod\;4699=3946
$$
<font size=4 face="楷体">	即密文为c = 3946；解密的过程为：</font>
$$
c^d\;mod\;N=3946^{349}\;mod\;4699=4583
$$


<font size=4 face="楷体" color=#00bfff>P8. Consider RSAwith p = 5 and q = 11. a. What are n and z? b. Let e be 3. Why is this an acceptable choice for e? c. Find d such that de = 1 (mod z) and d < 160. d. Encrypt the message m = 8 using the key (n, e). Let c denote the corresponding ciphertext. Show all work. Hint: To simplify the calculations, use the fact:
[(a mod n) • (b mod n)] mod n = (a • b) mod n</font>

<font size=4 face="楷体">	答、
	a、</font>
$$
n=p*q=5*11=55\\z=(p-1)*(q-1)=4*10=40
$$
<font size=4 face="楷体">	b、因为e取3满足e与z互素与e小于n两个条件；</font>

<font size=4 face="楷体">	c、可取d为27，这样就满足:
	$(ed)\;mod\;z=(3*27)\;mod\;40=81\;mod\;40=1$；</font>

<font size=4 face="楷体">	d、加密过程为：</font>
$$
m^e\;mod\;N=8^{3}\;mod\;55=(64*8)\;mod\;55=[(64\;mod\;55)*(8\;mod\;55)]mod\;55=72\;mod\;55=17
$$


<font size=4 face="楷体" color=#00bfff>P9. In this problem, we explore the Diffie-Hellman (DH) public-key encryption algorithm, which allows two entities to agree on a shared key. The DH algorithm makes use of a large prime number p and another large number g less than p. Both p and g are made public (so that an attacker would know them). In DH, Alice and Bob each independently choose secret keys, SA and SB, respectively. Alice then computes her public key, TA, by raising g to SA and then taking mod p. Bob similarly computes his own public key TB by raising g to SB and then taking mod p. Alice and Bob then exchange their public keys over the Internet. Alice then calculates the shared secret key S by raising TB to SA and then taking mod p. Similarly, Bob calculates the shared key S´ by raising TA to SB and then taking mod p. a. Prove that, in general, Alice and Bob obtain the same symmetric key, that is, prove S = S´. b. With p = 11 and g = 2, suppose Alice and Bob choose private keys SA = 5 and SB = 12, respectively. Calculate Alice’s and Bob’s public keys, TA and TB . Show all work. c. Following up on part (b), now calculate Sas the shared symmetric key. Show all work. d. Provide a timing diagram that shows how Diffie-Hellman can be attacked by a man-in-the-middle. The timing diagram should have three vertical lines, one for Alice, one for Bob, and one for the attacker Trudy. </font>

<font size=4 face="楷体">	答、</font>

<font size=4 face="楷体">	a、Alice的公钥为：</font>
$$
T_A=g^{S_A}\;mod\;p
$$
<font size=4 face="楷体">	Bob的公钥为：</font>
$$
T_B=g^{S_B}\;mod\;p
$$
<font size=4 face="楷体">	Alice计算共享密钥S为：</font>
$$
S={T_B}^{S_A}\;mod\;p=(g^{S_B}\;mod\;p)^{S_A}\;mod\;p=g^{S_AS_B}\;mod\;p
$$
<font size=4 face="楷体">	Bob计算共享密钥S'为：</font>
$$
S'={T_A}^{S_B}\;mod\;p=(g^{S_A}\;mod\;p)^{S_B}\;mod\;p=g^{S_AS_B}\;mod\;p
$$
<font size=4 face="楷体">	故$S=S'$;</font>

<font size=4 face="楷体">	b、Alice的公钥为：</font>
$$
T_A=g^{S_A}\;mod\;p=2^5\;mod\;11=10
$$
<font size=4 face="楷体">	Bob的公钥为：</font>
$$
T_B=g^{S_B}\;mod\;p=2^{12}\;mod\;11=4
$$
<font size=4 face="楷体">	c、由a过程可知对称密钥为：</font>
$$
S=g^{S_AS_B}\;mod\;p=2^{60}\;mod\;11=1
$$
<font size=4 face="楷体">	Alice计算共享密钥S为：</font>
$$
S={T_B}^{S_A}\;mod\;p=4^5\;mod\;11=1
$$
<font size=4 face="楷体">	Bob计算共享密钥S为：</font>
$$
S={T_A}^{S_B}\;mod\;p=10^{12}\;mod\;11=1
$$
<font size=4 face="楷体">	d、如下图所示：</font>

![](C:\Users\njuwhl2019hp\Desktop\计网\新文档 05-29-2020 22.40.19_2.jpg)

<font size=4 face="楷体">	即：Alice和Bob在交换公钥时，Trudy拦截TA，给Bob发送自己的公钥TT；拦截TB，给Alice发送自己的公钥TT；这样，Trudy与Alice、Trudy与Bob分别享有一个共享密钥，所以Trudy就可以解密所有Alice和Bob的信息。</font>



<font size=4 face="楷体" color=#00bfff>P16. Anatural question is whether we can use a nonce and public key cryptography to solve the end-point authentication problem in Section 8.4. Consider the following natural protocol: (1)  Alice sends the message “I am Alice” to Bob. (2) Bob chooses a nonce, R, and sends it to Alice. (3) Alice uses her private key to encrypt the nonce and sends the resulting value to Bob. (4) Bob applies Alice's public key to the received message. Thus, Bob computes R and authenticates Alice. a. Diagram this protocol, using the notation for public and private keys employed in the textbook. b. Suppose that certificates are not used. Describe how Trudy can become a “woman-in-the-middle” by intercepting Alice’s messages and then pretending to be Alice to Bob.</font>

<font size=4 face="楷体">	答、</font>

<font size=4 face="楷体">	a、如下图所示：</font>

![](C:\Users\njuwhl2019hp\Desktop\计网\新文档 05-29-2020 22.40.19_3.jpg)

<font size=4 face="楷体">	b、如果未使用证书的话，Bob就无法判断所谓的$K_A^+$是否真的是Alice的公钥；所以Trudy可以在Bob给Alice发送一个不重数R时拦截下来，用自己的私钥加密R，然后将加密后的信息发送给Bob，并声称自己的公钥就是$K_A^+$；Bob收到后，用$K_A^+$，即Trudy的公钥去计算，自然会得到R，进而鉴别Trudy就是Alice；因此Trudy对Bob假装她是Alice而成为中间人。</font>



<font size=4 face="楷体" color=#00bfff>P18. Suppose Alice wants to send an e-mail to Bob. Bob has a public-private key pair (KB +,KB –), and Alice has Bob’s certificate. But Alice does not have a public, private key pair. Alice and Bob (and the entire world) share the same hash function H(). a. In this situation, is it possible to design a scheme so that Bob can verify that Alice created the message? If so, show how with a block diagram for Alice and Bob. b. Is it possible to design a scheme that provides confidentiality for sending the message from Alice to Bob? If so, show how with a block diagram for Alice and Bob. </font>

​	<font size=4 face="楷体">答、
​	a、Alice没有公钥和私钥，因此无法让他人验证Alice创建的报文；
​	b、可以利用Bob的公钥和私钥保证通信的机密性（只能保证Alice向Bob发送的信息的机密性），具体而言就是用Bob的公钥加密发送的信息，Bob收到后用私钥解密：</font>

<img src="C:\Users\njuwhl2019hp\Desktop\计网\新文档 05-29-2020 22.40.19_4.jpg" style="zoom: 25%;" />



<font size=4 face="楷体" color=#00bfff>P19. Consider the Wireshark output below for a portion of an SSLsession. a. Is Wireshark packet 112 sent by the client or server? b. What is the server’s IPaddress and port number? c. Assuming no loss and no retransmissions, what will be the sequence number of the next TCPsegment sent by the client? d. How many SSLrecords does Wireshark packet 112 contain? e. Does packet 112 contain a Master Secret or an Encrypted Master Secret or neither? f. Assuming that the handshake type field is 1 byte and each length field is 3 bytes, what are the values of the first and last bytes of the Master Secret (or Encrypted Master Secret)? g. The client encrypted handshake message takes into account how many SSLrecords? h. The server encrypted handshake message takes into account how many SSLrecords? </font>

<font size=4 face="楷体">答、
	a、Client；由packet 106的Client Hello可以看出；
	b、IP地址：216.75.194.220：端口号：443（由packet 112信息可知）；
	c、由packet 112信息知该包`seq = 79,Len = 204`，故没有丢包或重传的话，客户发送的下一个TCP报文段的序号将是`79 + 204 = 283`；
	d、根据Secure Socket Layer显示，有3个SSL记录；
	e、显然112包是下图中的Round 3&4：</font>

<img src="C:\Users\njuwhl2019hp\Desktop\计网\作业.png" style="zoom:50%;" />

<font size=4 face="楷体">	在该阶段中，Client会生成一个PMS，并用服务器的公钥加密后发送，所以是包含一个加密的主密钥；
	f、根据wireshark底部显示，第1个字节表示握手类型，2-4字节是长度字段，后面的则为加密的主密钥，故第一个字节值为bc，最后一个字节值为29；
	g、6条，包括client Hello、server Hello、Certificate、server Hello Done、Client Key Exchange和Change Ciper Spec；
	h、8条，除了g中的6条还有Client的 Encrypted Handshake Message和Server的Change Ciper Spec。</font>

