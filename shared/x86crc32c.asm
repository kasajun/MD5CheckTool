TITLE	x86crc32c.asm
.686p
.xmm
include listing.inc
.model flat, c

CONST	SEGMENT
_short_shifts DQ 0dcb17aa400000000H
	DQ	0603ff91dbc8e83b9H
	DQ	0a0400b277cf17183H
	DQ	01cce889ec07ff23aH
	DQ	0255399a2f9e2e306H
	DQ	099dd1a1b456c60bfH
	DQ	059a2e82185139285H
	DQ	0e52c6b98399d113cH
	DQ	02a98ca59f629b0fdH
	DQ	0961649e04aa73344H
	DQ	05669bbda8ad8c17eH
	DQ	0eae73863365642c7H
	DQ	0d37a295f0fcb53fbH
	DQ	06ff4aae6b345d042H
	DQ	0af8b58dc733a2278H
	DQ	01305db65cfb4a1c1H
	DQ	0350e6dafe9bf170bH
	DQ	08980ee16553194b2H
	DQ	049ff1c2c954e6688H
	DQ	0f5719f9529c0e531H
	DQ	0ccec8ea9105df40dH
	DQ	070620d10acd377b4H
	DQ	0b01dff2a6cac858eH
	DQ	00c937c93d0220637H
	DQ	0c327dd521f96a7f6H
	DQ	07fa95eeba318244fH
	DQ	0bfd6acd16367d675H
	DQ	003582f68dfe955ccH
	DQ	03ac53e54e67444f0H
	DQ	0864bbded5afac749H
	DQ	046344fd79a853573H
	DQ	0fabacc6e260bb6caH
	DQ	00a232243d69258e7H
	DQ	0b6ada1fa6a1cdb5eH
	DQ	076d253c0aa632964H
	DQ	0ca5cd07916edaaddH
	DQ	0f3c1c1452f70bbe1H
	DQ	04f4f42fc93fe3858H
	DQ	08f30b0c65381ca62H
	DQ	033be337fef0f49dbH
	DQ	0fc0a92be20bbe81aH
	DQ	0408411079c356ba3H
	DQ	080fbe33d5c4a9999H
	DQ	03c756084e0c41a20H
	DQ	005e871b8d9590b1cH
	DQ	0b966f20165d788a5H
	DQ	07919003ba5a87a9fH
	DQ	0c59783821926f926H
	DQ	0e39c35483f2d4fecH
	DQ	05f12b6f183a3cc55H
	DQ	09f6d44cb43dc3e6fH
	DQ	023e3c772ff52bdd6H
	DQ	01a7ed64ec6cfaceaH
	DQ	0a6f055f77a412f53H
	DQ	0668fa7cdba3edd69H
	DQ	0da01247406b05ed0H
	DQ	015b585b5c904ff11H
	DQ	0a93b060c758a7ca8H
	DQ	06944f436b5f58e92H
	DQ	0d5ca778f097b0d2bH
	DQ	0ec5766b330e61c17H
	DQ	050d9e50a8c689faeH
	DQ	090a617304c176d94H
	DQ	02c289489f099ee2dH
	DQ	07479bd9ba8c8c73fH
	DQ	0c8f73e2214464486H
	DQ	00888cc18d439b6bcH
	DQ	0b4064fa168b73505H
	DQ	08d9b5e9d512a2439H
	DQ	03115dd24eda4a780H
	DQ	0f16a2f1e2ddb55baH
	DQ	04de4aca79155d603H
	DQ	082500d665ee177c2H
	DQ	03ede8edfe26ff47bH
	DQ	0fea17ce522100641H
	DQ	0422fff5c9e9e85f8H
	DQ	07bb2ee60a70394c4H
	DQ	0c73c6dd91b8d177dH
	DQ	007439fe3dbf2e547H
	DQ	0bbcd1c5a677c66feH
	DQ	09dc6aa904177d034H
	DQ	021482929fdf9538dH
	DQ	0e137db133d86a1b7H
	DQ	05db958aa8108220eH
	DQ	064244996b8953332H
	DQ	0d8aaca2f041bb08bH
	DQ	018d53815c46442b1H
	DQ	0a45bbbac78eac108H
	DQ	06bef1a6db75e60c9H
	DQ	0d76199d40bd0e370H
	DQ	0171e6beecbaf114aH
	DQ	0ab90e857772192f3H
	DQ	0920df96b4ebc83cfH
	DQ	02e837ad2f2320076H
	DQ	0eefc88e8324df24cH
	DQ	052720b518ec371f5H
	DQ	0a2ebe57c7e5a9fd8H
	DQ	01e6566c5c2d41c61H
	DQ	0de1a94ff02abee5bH
	DQ	062941746be256de2H
	DQ	05b09067a87b87cdeH
	DQ	0e78785c33b36ff67H
	DQ	027f877f9fb490d5dH
	DQ	09b76f44047c78ee4H
	DQ	054c2558188732f25H
	DQ	0e84cd63834fdac9cH
	DQ	028332402f4825ea6H
	DQ	094bda7bb480cdd1fH
	DQ	0ad20b6877191cc23H
	DQ	011ae353ecd1f4f9aH
	DQ	0d1d1c7040d60bda0H
	DQ	06d5f44bdb1ee3e19H
	DQ	04b54f27797e588d3H
	DQ	0f7da71ce2b6b0b6aH
	DQ	037a583f4eb14f950H
	DQ	08b2b004d579a7ae9H
	DQ	0b2b611716e076bd5H
	DQ	00e3892c8d289e86cH
	DQ	0ce4760f212f61a56H
	DQ	072c9e34bae7899efH
	DQ	0bd7d428a61cc382eH
	DQ	001f3c133dd42bb97H
	DQ	0c18c33091d3d49adH
	DQ	07d02b0b0a1b3ca14H
	DQ	0449fa18c982edb28H
	DQ	0f811223524a05891H
	DQ	0386ed00fe4dfaaabH
	DQ	084e053b658512912H
	DQ	0547df88f00000000H
	DQ	0fc860991a8fbf11eH
	DQ	000666c42541b94cdH
	DQ	0a89d9d5cfce065d3H
	DQ	0fc4ad115a837299aH
	DQ	054b1200b00ccd884H
	DQ	0a85145d8fc2cbd57H
	DQ	000aab4c654d74c49H
	DQ	001ffdd4a558225c5H
	DQ	0a9042c54fd79d4dbH
	DQ	055e449870199b108H
	DQ	0fd1fb899a9624016H
	DQ	0a9c8f4d0fdb50c5fH
	DQ	0013305ce554efd41H
	DQ	0fdd3601da9ae9892H
	DQ	0552891030155698cH
	DQ	0ff79b305ab044b8aH
	DQ	05782421b03ffba94H
	DQ	0ab6227c8ff1fdf47H
	DQ	00399d6d657e42e59H
	DQ	0574e9a9f03336210H
	DQ	0ffb56b81abc8930eH
	DQ	003550e525728f6ddH
	DQ	0abaeff4cffd307c3H
	DQ	0aafb96c0fe866e4fH
	DQ	0020067de567d9f51H
	DQ	0fee0020daa9dfa82H
	DQ	0561bf31302660b9cH
	DQ	002ccbf5a56b147d5H
	DQ	0aa374e44fe4ab6cbH
	DQ	056d72b9702aad318H
	DQ	0fe2cda89aa512206H
	DQ	00799196a53e4e1e5H
	DQ	0af62e874fb1f10fbH
	DQ	053828da707ff7528H
	DQ	0fb797cb9af048436H
	DQ	0afae30f0fbd3c87fH
	DQ	00755c1ee53283961H
	DQ	0fbb5a43dafc85cb2H
	DQ	0534e55230733adacH
	DQ	0521b3caf0666c420H
	DQ	0fae0cdb1ae9d353eH
	DQ	00600a862527d50edH
	DQ	0aefb597cfa86a1f3H
	DQ	0fa2c1535ae51edbaH
	DQ	052d7e42b06aa1ca4H
	DQ	0ae3781f8fa4a7977H
	DQ	006cc70e652b18869H
	DQ	0ac9d52e0f8e0aa6fH
	DQ	00466a3fe501b5b71H
	DQ	0f886c62dacfb3ea2H
	DQ	0507d37330400cfbcH
	DQ	004aa7b7a50d783f5H
	DQ	0ac518a64f82c72ebH
	DQ	050b1efb704cc1738H
	DQ	0f84a1ea9ac37e626H
	DQ	0f91f7725ad628faaH
	DQ	051e4863b05997eb4H
	DQ	0ad04e3e8f9791b67H
	DQ	005ff12f65182ea79H
	DQ	051285ebf0555a630H
	DQ	0f9d3afa1adae572eH
	DQ	00533ca72514e32fdH
	DQ	0adc83b6cf9b5c3e3H
	DQ	0f3b43b45a7c9c3caH
	DQ	05b4fca5b0f3232d4H
	DQ	0a7afaf88f3d25707H
	DQ	00f545e965b29a619H
	DQ	05b8312df0ffeea50H
	DQ	0f378e3c1a7051b4eH
	DQ	00f9886125be57e9dH
	DQ	0a763770cf31e8f83H
	DQ	0a6361e80f24be60fH
	DQ	00ecdef9e5ab01711H
	DQ	0f22d8a4da65072c2H
	DQ	05ad67b530eab83dcH
	DQ	00e01371a5a7ccf95H
	DQ	0a6fac604f2873e8bH
	DQ	05a1aa3d70e675b58H
	DQ	0f2e152c9a69caa46H
	DQ	058b070cf0ccd8840H
	DQ	0f04b81d1a436795eH
	DQ	00cabe40258d61c8dH
	DQ	0a450151cf02ded93H
	DQ	0f0875955a4faa1daH
	DQ	0587ca84b0c0150c4H
	DQ	0a49ccd98f0e13517H
	DQ	00c673c86581ac409H
	DQ	00d32550a594fad85H
	DQ	0a5c9a414f1b45c9bH
	DQ	05929c1c70d543948H
	DQ	0f1d230d9a5afc856H
	DQ	0a5057c90f178841fH
	DQ	00dfe8d8e59837501H
	DQ	0f11ee85da56310d2H
	DQ	059e519430d98e1ccH
	DQ	0a050daa0f42d222fH
	DQ	008ab2bbe5cd6d331H
	DQ	0f44b4e6da036b6e2H
	DQ	05cb0bf7308cd47fcH
	DQ	00867f33a5c1a0bb5H
	DQ	0a09c0224f4e1faabH
	DQ	05c7c67f708019f78H
	DQ	0f48796e9a0fa6e66H
	DQ	0f5d2ff65a1af07eaH
	DQ	05d290e7b0954f6f4H
	DQ	0a1c96ba8f5b49327H
	DQ	009329ab65d4f6239H
	DQ	05de5d6ff09982e70H
	DQ	0f51e27e1a163df6eH
	DQ	009fe42325d83babdH
	DQ	0a105b32cf5784ba3H
	DQ	00b54912a5f2969a5H
	DQ	0a3af6034f7d298bbH
	DQ	05f4f05e70b32fd68H
	DQ	0f7b4f4f9a3c90c76H
	DQ	0a363b8b0f71e403fH
	DQ	00b9849ae5fe5b121H
	DQ	0f7782c7da305d4f2H
	DQ	05f83dd630bfe25ecH
	DQ	05ed6b4ef0aab4c60H
	DQ	0f62d45f1a250bd7eH
	DQ	00acd20225eb0d8adH
	DQ	0a236d13cf64b29b3H
	DQ	0f6e19d75a29c65faH
	DQ	05e1a6c6b0a6794e4H
	DQ	0a2fa09b8f687f137H
	DQ	00a01f8a65e7c0029H
	DQ	04a7ff16500000000H
	DQ	0de8013af94ffe2caH
	DQ	0666c42002c13b365H
	DQ	0f293a0cab8ec51afH
	DQ	0125897af582766caH
	DQ	086a77565ccd88400H
	DQ	03e4b24ca7434d5afH
	DQ	0aab4c600e0cb3765H
	DQ	0fa313cf1b04ecd94H
	DQ	06ecede3b24b12f5eH
	DQ	0d6228f949c5d7ef1H
	DQ	042dd6d5e08a29c3bH
	DQ	0a2165a3be869ab5eH
	DQ	036e9b8f17c964994H
	DQ	08e05e95ec47a183bH
	DQ	01afa0b945085faf1H
	DQ	02f0e1cbc6571edd9H
	DQ	0bbf1fe76f18e0f13H
	DQ	0031dafd949625ebcH
	DQ	097e24d13dd9dbc76H
	DQ	077297a763d568b13H
	DQ	0e3d698bca9a969d9H
	DQ	05b3ac91311453876H
	DQ	0cfc52bd985badabcH
	DQ	09f40d128d53f204dH
	DQ	00bbf33e241c0c287H
	DQ	0b353624df92c9328H
	DQ	027ac80876dd371e2H
	DQ	0c767b7e28d184687H
	DQ	05398552819e7a44dH
	DQ	0eb740487a10bf5e2H
	DQ	07f8be64d35f41728H
	DQ	0809c2ad7cae3dbb2H
	DQ	01463c81d5e1c3978H
	DQ	0ac8f99b2e6f068d7H
	DQ	038707b78720f8a1dH
	DQ	0d8bb4c1d92c4bd78H
	DQ	04c44aed7063b5fb2H
	DQ	0f4a8ff78bed70e1dH
	DQ	060571db22a28ecd7H
	DQ	030d2e7437aad1626H
	DQ	0a42d0589ee52f4ecH
	DQ	01cc1542656bea543H
	DQ	0883eb6ecc2414789H
	DQ	068f58189228a70ecH
	DQ	0fc0a6343b6759226H
	DQ	044e632ec0e99c389H
	DQ	0d019d0269a662143H
	DQ	0e5edc70eaf92366bH
	DQ	0711225c43b6dd4a1H
	DQ	0c9fe746b8381850eH
	DQ	05d0196a1177e67c4H
	DQ	0bdcaa1c4f7b550a1H
	DQ	02935430e634ab26bH
	DQ	091d912a1dba6e3c4H
	DQ	00526f06b4f59010eH
	DQ	055a30a9a1fdcfbffH
	DQ	0c15ce8508b231935H
	DQ	079b0b9ff33cf489aH
	DQ	0ed4f5b35a730aa50H
	DQ	00d846c5047fb9d35H
	DQ	0997b8e9ad3047fffH
	DQ	02197df356be82e50H
	DQ	0b5683dffff17cc9aH
	DQ	0da5430f0902bc195H
	DQ	04eabd23a04d4235fH
	DQ	0f6478395bc3872f0H
	DQ	062b8615f28c7903aH
	DQ	08273563ac80ca75fH
	DQ	0168cb4f05cf34595H
	DQ	0ae60e55fe41f143aH
	DQ	03a9f079570e0f6f0H
	DQ	06a1afd6420650c01H
	DQ	0fee51faeb49aeecbH
	DQ	046094e010c76bf64H
	DQ	0d2f6accb98895daeH
	DQ	0323d9bae78426acbH
	DQ	0a6c27964ecbd8801H
	DQ	01e2e28cb5451d9aeH
	DQ	08ad1ca01c0ae3b64H
	DQ	0bf25dd29f55a2c4cH
	DQ	02bda3fe361a5ce86H
	DQ	093366e4cd9499f29H
	DQ	007c98c864db67de3H
	DQ	0e702bbe3ad7d4a86H
	DQ	073fd59293982a84cH
	DQ	0cb110886816ef9e3H
	DQ	05feeea4c15911b29H
	DQ	00f6b10bd4514e1d8H
	DQ	09b94f277d1eb0312H
	DQ	02378a3d8690752bdH
	DQ	0b7874112fdf8b077H
	DQ	0574c76771d338712H
	DQ	0c3b394bd89cc65d8H
	DQ	07b5fc51231203477H
	DQ	0efa027d8a5dfd6bdH
	DQ	010b7eb425ac81a27H
	DQ	084480988ce37f8edH
	DQ	03ca4582776dba942H
	DQ	0a85bbaede2244b88H
	DQ	048908d8802ef7cedH
	DQ	0dc6f6f4296109e27H
	DQ	064833eed2efccf88H
	DQ	0f07cdc27ba032d42H
	DQ	0a0f926d6ea86d7b3H
	DQ	03406c41c7e793579H
	DQ	08cea95b3c69564d6H
	DQ	018157779526a861cH
	DQ	0f8de401cb2a1b179H
	DQ	06c21a2d6265e53b3H
	DQ	0d4cdf3799eb2021cH
	DQ	0403211b30a4de0d6H
	DQ	075c6069b3fb9f7feH
	DQ	0e139e451ab461534H
	DQ	059d5b5fe13aa449bH
	DQ	0cd2a57348755a651H
	DQ	02de16051679e9134H
	DQ	0b91e829bf36173feH
	DQ	001f2d3344b8d2251H
	DQ	0950d31fedf72c09bH
	DQ	0c588cb0f8ff73a6aH
	DQ	0517729c51b08d8a0H
	DQ	0e99b786aa3e4890fH
	DQ	07d649aa0371b6bc5H
	DQ	09dafadc5d7d05ca0H
	DQ	009504f0f432fbe6aH
	DQ	0b1bc1ea0fbc3efc5H
	DQ	02543fc6a6f3c0d0fH
	DQ	025bbf5db00000000H
	DQ	06ecc1e6d4b77ebb6H
	DQ	0b35422b796efd76cH
	DQ	0f823c901dd983cdaH
	DQ	00d882df22833d829H
	DQ	046ffc6446344339fH
	DQ	09b67fa9ebedc0f45H
	DQ	0d0101128f5abe4f3H
	DQ	075dc45895067b052H
	DQ	03eabae3f1b105be4H
	DQ	0e33392e5c688673eH
	DQ	0a84479538dff8c88H
	DQ	05def9da07854687bH
	DQ	016987616332383cdH
	DQ	0cb004acceebbbf17H
	DQ	08077a17aa5cc54a1H
	DQ	08574957fa0cf60a4H
	DQ	0ce037ec9ebb88b12H
	DQ	0139b42133620b7c8H
	DQ	058eca9a57d575c7eH
	DQ	0ad474d5688fcb88dH
	DQ	0e630a6e0c38b533bH
	DQ	03ba89a3a1e136fe1H
	DQ	070df718c55648457H
	DQ	0d513252df0a8d0f6H
	DQ	09e64ce9bbbdf3b40H
	DQ	043fcf2416647079aH
	DQ	0088b19f72d30ec2cH
	DQ	0fd20fd04d89b08dfH
	DQ	0b65716b293ece369H
	DQ	06bcf2a684e74dfb3H
	DQ	020b8c1de05033405H
	DQ	061c942624472b7b9H
	DQ	02abea9d40f055c0fH
	DQ	0f726950ed29d60d5H
	DQ	0bc517eb899ea8b63H
	DQ	049fa9a4b6c416f90H
	DQ	0028d71fd27368426H
	DQ	0df154d27faaeb8fcH
	DQ	09462a691b1d9534aH
	DQ	031aef230141507ebH
	DQ	07ad919865f62ec5dH
	DQ	0a741255c82fad087H
	DQ	0ec36ceeac98d3b31H
	DQ	0199d2a193c26dfc2H
	DQ	052eac1af77513474H
	DQ	08f72fd75aac908aeH
	DQ	0c40516c3e1bee318H
	DQ	0c10622c6e4bdd71dH
	DQ	08a71c970afca3cabH
	DQ	057e9f5aa72520071H
	DQ	01c9e1e1c3925ebc7H
	DQ	0e935faefcc8e0f34H
	DQ	0a242115987f9e482H
	DQ	07fda2d835a61d858H
	DQ	034adc635111633eeH
	DQ	091619294b4da674fH
	DQ	0da167922ffad8cf9H
	DQ	0078e45f82235b023H
	DQ	04cf9ae4e69425b95H
	DQ	0b9524abd9ce9bf66H
	DQ	0f225a10bd79e54d0H
	DQ	02fbd9dd10a06680aH
	DQ	064ca7667417183bcH
	DQ	0ad5e9aa988e56f72H
	DQ	0e629711fc39284c4H
	DQ	03bb14dc51e0ab81eH
	DQ	070c6a673557d53a8H
	DQ	0856d4280a0d6b75bH
	DQ	0ce1aa936eba15cedH
	DQ	0138295ec36396037H
	DQ	058f57e5a7d4e8b81H
	DQ	0fd392afbd882df20H
	DQ	0b64ec14d93f53496H
	DQ	06bd6fd974e6d084cH
	DQ	020a11621051ae3faH
	DQ	0d50af2d2f0b10709H
	DQ	09e7d1964bbc6ecbfH
	DQ	043e525be665ed065H
	DQ	00892ce082d293bd3H
	DQ	00d91fa0d282a0fd6H
	DQ	046e611bb635de460H
	DQ	09b7e2d61bec5d8baH
	DQ	0d009c6d7f5b2330cH
	DQ	025a222240019d7ffH
	DQ	06ed5c9924b6e3c49H
	DQ	0b34df54896f60093H
	DQ	0f83a1efedd81eb25H
	DQ	05df64a5f784dbf84H
	DQ	01681a1e9333a5432H
	DQ	0cb199d33eea268e8H
	DQ	0806e7685a5d5835eH
	DQ	075c59276507e67adH
	DQ	03eb279c01b098c1bH
	DQ	0e32a451ac691b0c1H
	DQ	0a85daeac8de65b77H
	DQ	0e92c2d10cc97d8cbH
	DQ	0a25bc6a687e0337dH
	DQ	07fc3fa7c5a780fa7H
	DQ	034b411ca110fe411H
	DQ	0c11ff539e4a400e2H
	DQ	08a681e8fafd3eb54H
	DQ	057f02255724bd78eH
	DQ	01c87c9e3393c3c38H
	DQ	0b94b9d429cf06899H
	DQ	0f23c76f4d787832fH
	DQ	02fa44a2e0a1fbff5H
	DQ	064d3a19841685443H
	DQ	09178456bb4c3b0b0H
	DQ	0da0faeddffb45b06H
	DQ	007979207222c67dcH
	DQ	04ce079b1695b8c6aH
	DQ	049e34db46c58b86fH
	DQ	00294a602272f53d9H
	DQ	0df0c9ad8fab76f03H
	DQ	0947b716eb1c084b5H
	DQ	061d0959d446b6046H
	DQ	02aa77e2b0f1c8bf0H
	DQ	0f73f42f1d284b72aH
	DQ	0bc48a94799f35c9cH
	DQ	01984fde63c3f083dH
	DQ	052f316507748e38bH
	DQ	08f6b2a8aaad0df51H
	DQ	0c41cc13ce1a734e7H
	DQ	031b725cf140cd014H
	DQ	07ac0ce795f7b3ba2H
	DQ	0a758f2a382e30778H
	DQ	0ec2f1915c994ecceH

_long_shifts DQ	0e040e0ac00000000H
	DQ	0252d5705c56db7a9H
	DQ	06f77f90f8f3719a3H
	DQ	0aa1a4ea64a5aae0aH
	DQ	0fbc2a51b1b8245b7H
	DQ	03eaf12b2deeff21eH
	DQ	074f5bcb894b55c14H
	DQ	0b1980b1151d8ebbdH
	DQ	0d7446bc237048b6eH
	DQ	01229dc6bf2693cc7H
	DQ	058737261b83392cdH
	DQ	09d1ec5c87d5e2564H
	DQ	0ccc62e752c86ced9H
	DQ	009ab99dce9eb7970H
	DQ	043f137d6a3b1d77aH
	DQ	0869c807f66dc60d3H
	DQ	08e49f6706e0916dcH
	DQ	04b2441d9ab64a175H
	DQ	0017eefd3e13e0f7fH
	DQ	0c413587a2453b8d6H
	DQ	095cbb3c7758b536bH
	DQ	050a6046eb0e6e4c2H
	DQ	01afcaa64fabc4ac8H
	DQ	0df911dcd3fd1fd61H
	DQ	0b94d7d1e590d9db2H
	DQ	07c20cab79c602a1bH
	DQ	0367a64bdd63a8411H
	DQ	0f317d314135733b8H
	DQ	0a2cf38a9428fd805H
	DQ	067a28f0087e26facH
	DQ	02df8210acdb8c1a6H
	DQ	0e89596a308d5760fH
	DQ	03c52cd14dc122db8H
	DQ	0f93f7abd197f9a11H
	DQ	0b365d4b75325341bH
	DQ	07608631e964883b2H
	DQ	027d088a3c790680fH
	DQ	0e2bd3f0a02fddfa6H
	DQ	0a8e7910048a771acH
	DQ	06d8a26a98dcac605H
	DQ	00b56467aeb16a6d6H
	DQ	0ce3bf1d32e7b117fH
	DQ	084615fd96421bf75H
	DQ	0410ce870a14c08dcH
	DQ	010d403cdf094e361H
	DQ	0d5b9b46435f954c8H
	DQ	09fe31a6e7fa3fac2H
	DQ	05a8eadc7bace4d6bH
	DQ	0525bdbc8b21b3b64H
	DQ	097366c6177768ccdH
	DQ	0dd6cc26b3d2c22c7H
	DQ	0180175c2f841956eH
	DQ	049d99e7fa9997ed3H
	DQ	08cb429d66cf4c97aH
	DQ	0c6ee87dc26ae6770H
	DQ	003833075e3c3d0d9H
	DQ	0655f50a6851fb00aH
	DQ	0a032e70f407207a3H
	DQ	0ea6849050a28a9a9H
	DQ	02f05feaccf451e00H
	DQ	07edd15119e9df5bdH
	DQ	0bbb0a2b85bf04214H
	DQ	0f1ea0cb211aaec1eH
	DQ	03487bb1bd4c75bb7H
	DQ	05d88cd2dbdc82d81H
	DQ	098e57a8478a59a28H
	DQ	0d2bfd48e32ff3422H
	DQ	017d26327f792838bH
	DQ	0460a889aa64a6836H
	DQ	083673f336327df9fH
	DQ	0c93d9139297d7195H
	DQ	00c502690ec10c63cH
	DQ	06a8c46438acca6efH
	DQ	0afe1f1ea4fa11146H
	DQ	0e5bb5fe005fbbf4cH
	DQ	020d6e849c09608e5H
	DQ	0710e03f4914ee358H
	DQ	0b463b45d542354f1H
	DQ	0fe391a571e79fafbH
	DQ	03b54adfedb144d52H
	DQ	03381dbf1d3c13b5dH
	DQ	0f6ec6c5816ac8cf4H
	DQ	0bcb6c2525cf622feH
	DQ	079db75fb999b9557H
	DQ	028039e46c8437eeaH
	DQ	0ed6e29ef0d2ec943H
	DQ	0a73487e547746749H
	DQ	06259304c8219d0e0H
	DQ	00485509fe4c5b033H
	DQ	0c1e8e73621a8079aH
	DQ	08bb2493c6bf2a990H
	DQ	04edffe95ae9f1e39H
	DQ	01f071528ff47f584H
	DQ	0da6aa2813a2a422dH
	DQ	090300c8b7070ec27H
	DQ	0555dbb22b51d5b8eH
	DQ	0819ae09561da0039H
	DQ	044f7573ca4b7b790H
	DQ	00eadf936eeed199aH
	DQ	0cbc04e9f2b80ae33H
	DQ	09a18a5227a58458eH
	DQ	05f75128bbf35f227H
	DQ	0152fbc81f56f5c2dH
	DQ	0d0420b283002eb84H
	DQ	0b69e6bfb56de8b57H
	DQ	073f3dc5293b33cfeH
	DQ	039a97258d9e992f4H
	DQ	0fcc4c5f11c84255dH
	DQ	0ad1c2e4c4d5ccee0H
	DQ	0687199e588317949H
	DQ	0222b37efc26bd743H
	DQ	0e7468046070660eaH
	DQ	0ef93f6490fd316e5H
	DQ	02afe41e0cabea14cH
	DQ	060a4efea80e40f46H
	DQ	0a5c958434589b8efH
	DQ	0f411b3fe14515352H
	DQ	0317c0457d13ce4fbH
	DQ	07b26aa5d9b664af1H
	DQ	0be4b1df45e0bfd58H
	DQ	0d8977d2738d79d8bH
	DQ	01dfaca8efdba2a22H
	DQ	057a06484b7e08428H
	DQ	092cdd32d728d3381H
	DQ	0c31538902355d83cH
	DQ	006788f39e6386f95H
	DQ	04c222133ac62c19fH
	DQ	0894f969a690f7636H
	DQ	07e7c2df300000000H
	DQ	082847615fcf85be6H
	DQ	08260eccefc1cc13dH
	DQ	07e98b72800e49adbH
	DQ	083a9d978fdd5f48bH
	DQ	07f51829e012daf6dH
	DQ	07fb5184501c935b6H
	DQ	0834d43a3fd316e50H
	DQ	0803bb214fe479fe7H
	DQ	07cc3e9f202bfc401H
	DQ	07c277329025b5edaH
	DQ	080df28cffea3053cH
	DQ	07dee469f03926b6cH
	DQ	081161d79ff6a308aH
	DQ	081f287a2ff8eaa51H
	DQ	07d0adc440376f1b7H
	DQ	0871f64ccf963493fH
	DQ	07be73f2a059b12d9H
	DQ	07b03a5f1057f8802H
	DQ	087fbfe17f987d3e4H
	DQ	07aca904704b6bdb4H
	DQ	08632cba1f84ee652H
	DQ	086d6517af8aa7c89H
	DQ	07a2e0a9c0452276fH
	DQ	07958fb2b0724d6d8H
	DQ	085a0a0cdfbdc8d3eH
	DQ	085443a16fb3817e5H
	DQ	079bc61f007c04c03H
	DQ	0848d0fa0faf12253H
	DQ	078755446060979b5H
	DQ	07891ce9d06ede36eH
	DQ	08469957bfa15b888H
	DQ	08956c97cf72ae48fH
	DQ	075ae929a0bd2bf69H
	DQ	0754a08410b3625b2H
	DQ	089b253a7f7ce7e54H
	DQ	074833df70aff1004H
	DQ	0887b6611f6074be2H
	DQ	0889ffccaf6e3d139H
	DQ	07467a72c0a1b8adfH
	DQ	07711569b096d7b68H
	DQ	08be90d7df595208eH
	DQ	08b0d97a6f571ba55H
	DQ	077f5cc400989e1b3H
	DQ	08ac4a210f4b88fe3H
	DQ	0763cf9f60840d405H
	DQ	076d8632d08a44edeH
	DQ	08a2038cbf45c1538H
	DQ	0703580430e49adb0H
	DQ	08ccddba5f2b1f656H
	DQ	08c29417ef2556c8dH
	DQ	070d11a980ead376bH
	DQ	08de074c8f39c593bH
	DQ	071182f2e0f6402ddH
	DQ	071fcb5f50f809806H
	DQ	08d04ee13f378c3e0H
	DQ	08e721fa4f00e3257H
	DQ	0728a44420cf669b1H
	DQ	0726ede990c12f36aH
	DQ	08e96857ff0eaa88cH
	DQ	073a7eb2f0ddbc6dcH
	DQ	08f5fb0c9f1239d3aH
	DQ	08fbb2a12f1c707e1H
	DQ	0734371f40d3f5c07H
	DQ	095c5921cebb9bfefH
	DQ	0693dc9fa1741e409H
	DQ	069d9532117a57ed2H
	DQ	0952108c7eb5d2534H
	DQ	068106697166c4b64H
	DQ	094e83d71ea941082H
	DQ	0940ca7aaea708a59H
	DQ	068f4fc4c1688d1bfH
	DQ	06b820dfb15fe2008H
	DQ	0977a561de9067beeH
	DQ	0979eccc6e9e2e135H
	DQ	06b669720151abad3H
	DQ	09657f970e82bd483H
	DQ	06aafa29614d38f65H
	DQ	06a4b384d143715beH
	DQ	096b363abe8cf4e58H
	DQ	06ca6db2312daf6d0H
	DQ	0905e80c5ee22ad36H
	DQ	090ba1a1eeec637edH
	DQ	06c4241f8123e6c0bH
	DQ	091732fa8ef0f025bH
	DQ	06d8b744e13f759bdH
	DQ	06d6fee951313c366H
	DQ	09197b573efeb9880H
	DQ	092e144c4ec9d6937H
	DQ	06e191f22106532d1H
	DQ	06efd85f91081a80aH
	DQ	09205de1fec79f3ecH
	DQ	06f34b04f11489dbcH
	DQ	093cceba9edb0c65aH
	DQ	093287172ed545c81H
	DQ	06fd02a9411ac0767H
	DQ	062ef76931c935b60H
	DQ	09e172d75e06b0086H
	DQ	09ef3b7aee08f9a5dH
	DQ	0620bec481c77c1bbH
	DQ	09f3a8218e146afebH
	DQ	063c2d9fe1dbef40dH
	DQ	0632643251d5a6ed6H
	DQ	09fde18c3e1a23530H
	DQ	09ca8e974e2d4c487H
	DQ	06050b2921e2c9f61H
	DQ	060b428491ec805baH
	DQ	09c4c73afe2305e5cH
	DQ	0617d1dff1f01300cH
	DQ	09d854619e3f96beaH
	DQ	09d61dcc2e31df131H
	DQ	0619987241fe5aad7H
	DQ	09b8c3face5f0125fH
	DQ	06774644a190849b9H
	DQ	06790fe9119ecd362H
	DQ	09b68a577e5148884H
	DQ	06659cb271825e6d4H
	DQ	09aa190c1e4ddbd32H
	DQ	09a450a1ae43927e9H
	DQ	066bd51fc18c17c0fH
	DQ	065cba04b1bb78db8H
	DQ	09933fbade74fd65eH
	DQ	099d76176e7ab4c85H
	DQ	0652f3a901b531763H
	DQ	0981e54c0e6627933H
	DQ	064e60f261a9a22d5H
	DQ	0640295fd1a7eb80eH
	DQ	098face1be686e3e8H
	DQ	0d29f092f00000000H
	DQ	0724d6d80a0d264afH
	DQ	096d7b6804448bfafH
	DQ	03605d22fe49adb00H
	DQ	05a0e767188917f5eH
	DQ	0fadc12de28431bf1H
	DQ	01e46c9deccd9c0f1H
	DQ	0be94ad716c0ba45eH
	DQ	0c651816214ce884dH
	DQ	06683e5cdb41cece2H
	DQ	082193ecd508637e2H
	DQ	022cb5a62f054534dH
	DQ	04ec0fe3c9c5ff713H
	DQ	0ee129a933c8d93bcH
	DQ	00a884193d81748bcH
	DQ	0aa5a253c78c52c13H
	DQ	0fb0219b5299d109aH
	DQ	05bd07d1a894f7435H
	DQ	0bf4aa61a6dd5af35H
	DQ	01f98c2b5cd07cb9aH
	DQ	0739366eba10c6fc4H
	DQ	0d341024401de0b6bH
	DQ	037dbd944e544d06bH
	DQ	09709bdeb4596b4c4H
	DQ	0efcc91f83d5398d7H
	DQ	04f1ef5579d81fc78H
	DQ	0ab842e57791b2778H
	DQ	00b564af8d9c943d7H
	DQ	0675deea6b5c2e789H
	DQ	0c78f8a0915108326H
	DQ	023155109f18a5826H
	DQ	083c735a651583c89H
	DQ	081a5281b533a2134H
	DQ	021774cb4f3e8459bH
	DQ	0c5ed97b417729e9bH
	DQ	0653ff31bb7a0fa34H
	DQ	009345745dbab5e6aH
	DQ	0a9e633ea7b793ac5H
	DQ	04d7ce8ea9fe3e1c5H
	DQ	0edae8c453f31856aH
	DQ	0956ba05647f4a979H
	DQ	035b9c4f9e726cdd6H
	DQ	0d1231ff903bc16d6H
	DQ	071f17b56a36e7279H
	DQ	01dfadf08cf65d627H
	DQ	0bd28bba76fb7b288H
	DQ	059b260a78b2d6988H
	DQ	0f96004082bff0d27H
	DQ	0a83838817aa731aeH
	DQ	008ea5c2eda755501H
	DQ	0ec70872e3eef8e01H
	DQ	04ca2e3819e3deaaeH
	DQ	020a947dff2364ef0H
	DQ	0807b237052e42a5fH
	DQ	064e1f870b67ef15fH
	DQ	0c4339cdf16ac95f0H
	DQ	0bcf6b0cc6e69b9e3H
	DQ	01c24d463cebbdd4cH
	DQ	0f8be0f632a21064cH
	DQ	0586c6bcc8af362e3H
	DQ	03467cf92e6f8c6bdH
	DQ	094b5ab3d462aa212H
	DQ	0702f703da2b07912H
	DQ	0d0fd149202621dbdH
	DQ	074eb4b47a6744268H
	DQ	0d4392fe806a626c7H
	DQ	030a3f4e8e23cfdc7H
	DQ	09071904742ee9968H
	DQ	0fc7a34192ee53d36H
	DQ	05ca850b68e375999H
	DQ	0b8328bb66aad8299H
	DQ	018e0ef19ca7fe636H
	DQ	06025c30ab2baca25H
	DQ	0c0f7a7a51268ae8aH
	DQ	0246d7ca5f6f2758aH
	DQ	084bf180a56201125H
	DQ	0e8b4bc543a2bb57bH
	DQ	04866d8fb9af9d1d4H
	DQ	0acfc03fb7e630ad4H
	DQ	00c2e6754deb16e7bH
	DQ	05d765bdd8fe952f2H
	DQ	0fda43f722f3b365dH
	DQ	0193ee472cba1ed5dH
	DQ	0b9ec80dd6b7389f2H
	DQ	0d5e7248307782dacH
	DQ	07535402ca7aa4903H
	DQ	091af9b2c43309203H
	DQ	0317dff83e3e2f6acH
	DQ	049b8d3909b27dabfH
	DQ	0e96ab73f3bf5be10H
	DQ	00df06c3fdf6f6510H
	DQ	0ad2208907fbd01bfH
	DQ	0c129acce13b6a5e1H
	DQ	061fbc861b364c14eH
	DQ	08561136157fe1a4eH
	DQ	025b377cef72c7ee1H
	DQ	027d16a73f54e635cH
	DQ	087030edc559c07f3H
	DQ	06399d5dcb106dcf3H
	DQ	0c34bb17311d4b85cH
	DQ	0af40152d7ddf1c02H
	DQ	00f927182dd0d78adH
	DQ	0eb08aa823997a3adH
	DQ	04bdace2d9945c702H
	DQ	0331fe23ee180eb11H
	DQ	093cd869141528fbeH
	DQ	077575d91a5c854beH
	DQ	0d785393e051a3011H
	DQ	0bb8e9d606911944fH
	DQ	01b5cf9cfc9c3f0e0H
	DQ	0ffc622cf2d592be0H
	DQ	05f1446608d8b4f4fH
	DQ	00e4c7ae9dcd373c6H
	DQ	0ae9e1e467c011769H
	DQ	04a04c546989bcc69H
	DQ	0ead6a1e93849a8c6H
	DQ	086dd05b754420c98H
	DQ	0260f6118f4906837H
	DQ	0c295ba18100ab337H
	DQ	06247deb7b0d8d798H
	DQ	01a82f2a4c81dfb8bH
	DQ	0ba50960b68cf9f24H
	DQ	05eca4d0b8c554424H
	DQ	0fe1829a42c87208bH
	DQ	092138dfa408c84d5H
	DQ	032c1e955e05ee07aH
	DQ	0d65b325504c43b7aH
	DQ	0768956faa4165fd5H
	DQ	04904f22100000000H
	DQ	0db0d16639209e442H
	DQ	068fb4c5421ffbe75H
	DQ	0faf2a816b3f65a37H
	DQ	00afb8ecb43ff7ceaH
	DQ	098f26a89d1f698a8H
	DQ	02b0430be6200c29fH
	DQ	0b90dd4fcf00926ddH
	DQ	0cefa0bf587fef9d4H
	DQ	05cf3efb715f71d96H
	DQ	0ef05b580a60147a1H
	DQ	07d0c51c23408a3e3H
	DQ	08d05771fc401853eH
	DQ	01f0c935d5608617cH
	DQ	0acfac96ae5fe3b4bH
	DQ	03ef32d2877f7df09H
	DQ	0431577780a118559H
	DQ	0d11c933a9818611bH
	DQ	062eac90d2bee3b2cH
	DQ	0f0e32d4fb9e7df6eH
	DQ	000ea0b9249eef9b3H
	DQ	092e3efd0dbe71df1H
	DQ	02115b5e7681147c6H
	DQ	0b31c51a5fa18a384H
	DQ	0c4eb8eac8def7c8dH
	DQ	056e26aee1fe698cfH
	DQ	0e51430d9ac10c2f8H
	DQ	0771dd49b3e1926baH
	DQ	08714f246ce100067H
	DQ	0151d16045c19e425H
	DQ	0a6eb4c33efefbe12H
	DQ	034e2a8717de65a50H
	DQ	05d27f89314230ab2H
	DQ	0cf2e1cd1862aeef0H
	DQ	07cd846e635dcb4c7H
	DQ	0eed1a2a4a7d55085H
	DQ	01ed8847957dc7658H
	DQ	08cd1603bc5d5921aH
	DQ	03f273a0c7623c82dH
	DQ	0ad2ede4ee42a2c6fH
	DQ	0dad9014793ddf366H
	DQ	048d0e50501d41724H
	DQ	0fb26bf32b2224d13H
	DQ	0692f5b70202ba951H
	DQ	099267dadd0228f8cH
	DQ	00b2f99ef422b6bceH
	DQ	0b8d9c3d8f1dd31f9H
	DQ	02ad0279a63d4d5bbH
	DQ	057367dca1e328febH
	DQ	0c53f99888c3b6ba9H
	DQ	076c9c3bf3fcd319eH
	DQ	0e4c027fdadc4d5dcH
	DQ	014c901205dcdf301H
	DQ	086c0e562cfc41743H
	DQ	03536bf557c324d74H
	DQ	0a73f5b17ee3ba936H
	DQ	0d0c8841e99cc763fH
	DQ	042c1605c0bc5927dH
	DQ	0f1373a6bb833c84aH
	DQ	0633ede292a3a2c08H
	DQ	09337f8f4da330ad5H
	DQ	0013e1cb6483aee97H
	DQ	0b2c84681fbccb4a0H
	DQ	020c1a2c369c550e2H
	DQ	06142e74528461564H
	DQ	0f34b0307ba4ff126H
	DQ	040bd593009b9ab11H
	DQ	0d2b4bd729bb04f53H
	DQ	022bd9baf6bb9698eH
	DQ	0b0b47fedf9b08dccH
	DQ	0034225da4a46d7fbH
	DQ	0914bc198d84f33b9H
	DQ	0e6bc1e91afb8ecb0H
	DQ	074b5fad33db108f2H
	DQ	0c743a0e48e4752c5H
	DQ	0554a44a61c4eb687H
	DQ	0a543627bec47905aH
	DQ	0374a86397e4e7418H
	DQ	084bcdc0ecdb82e2fH
	DQ	016b5384c5fb1ca6dH
	DQ	06b53621c2257903dH
	DQ	0f95a865eb05e747fH
	DQ	04aacdc6903a82e48H
	DQ	0d8a5382b91a1ca0aH
	DQ	028ac1ef661a8ecd7H
	DQ	0baa5fab4f3a10895H
	DQ	00953a083405752a2H
	DQ	09b5a44c1d25eb6e0H
	DQ	0ecad9bc8a5a969e9H
	DQ	07ea47f8a37a08dabH
	DQ	0cd5225bd8456d79cH
	DQ	05f5bc1ff165f33deH
	DQ	0af52e722e6561503H
	DQ	03d5b0360745ff141H
	DQ	08ead5957c7a9ab76H
	DQ	01ca4bd1555a04f34H
	DQ	07561edf73c651fd6H
	DQ	0e76809b5ae6cfb94H
	DQ	0549e53821d9aa1a3H
	DQ	0c697b7c08f9345e1H
	DQ	0369e911d7f9a633cH
	DQ	0a497755fed93877eH
	DQ	017612f685e65dd49H
	DQ	08568cb2acc6c390bH
	DQ	0f29f1423bb9be602H
	DQ	06096f06129920240H
	DQ	0d360aa569a645877H
	DQ	041694e14086dbc35H
	DQ	0b16068c9f8649ae8H
	DQ	023698c8b6a6d7eaaH
	DQ	0909fd6bcd99b249dH
	DQ	0029632fe4b92c0dfH
	DQ	07f7068ae36749a8fH
	DQ	0ed798ceca47d7ecdH
	DQ	05e8fd6db178b24faH
	DQ	0cc8632998582c0b8H
	DQ	03c8f1444758be665H
	DQ	0ae86f006e7820227H
	DQ	01d70aa3154745810H
	DQ	08f794e73c67dbc52H
	DQ	0f88e917ab18a635bH
	DQ	06a87753823838719H
	DQ	0d9712f0f9075dd2eH
	DQ	04b78cb4d027c396cH
	DQ	0bb71ed90f2751fb1H
	DQ	0297809d2607cfbf3H
	DQ	09a8e53e5d38aa1c4H
	DQ	00887b7a741834586H
CONST	ENDS

.code

; void __stdcall crc32c_asm(unsigned int crc, const unsigned char* buf, size_t len);
crc32c_asm proc stdcall crc:dword, buf:dword, len:dword
	sub		esp, 8
	push	ebx						;	//	EAX, ECX, EDX 以外は退避する
	push	esi						;	//	EBX は盲点なので注意->原因不明の強制終了に
	push	edi
									;	//	Cソースコード
									;	//	厳密な型がないので基本的にDWORD (unsigned int) です。
									;	unsigned int *next, crc0, *end, end1, temp, ret;
	mov		esi, dword ptr [buf]	;	next = buf;
	mov		ebx, dword ptr [crc]	;	crc0 = *crc;
	mov		edx, dword ptr [len]	;	end1 = *len;
	not		ebx						;	crc0 ~= crc0;
	cmp		edx, 24576
	jb		crc32_asm_longshift_end

	mov		eax, -1431655765
	mul		edx
	shr		edx, 14
	mov		dword ptr [ebp - 4], edx
	npad	5

crc32_asm_longshift_start:
	xor		edi, edi
	lea		ecx, dword ptr [esi + 8192]
	xor		eax, eax
	npad	6

crc32_asm_longshift_loop_start:
	crc32	ebx,  dword ptr [esi]
	crc32	edi,  dword ptr [esi + 8192]
	crc32	eax,  dword ptr [esi + 16384]
	add		esi, 4
	cmp		esi, ecx
	jb		short crc32_asm_longshift_loop_start;

crc32_asm_longshift_loop_end:
	mov		dword ptr [ebp - 8], eax
	add		esi, 16384
	mov		eax, ebx
	shr		eax, 16
	movzx	ecx, al
	mov		eax, ebx
	shr		eax, 8
	movzx	eax, al
	mov		edx, dword ptr _long_shifts[ecx * 4 + 2048]
	xor		edx, dword ptr _long_shifts[eax * 4 + 1024]
	mov		eax, ebx
	shr		eax, 24
	xor		edx, dword ptr _long_shifts[eax * 4 + 3072]
	movzx	eax, bl
	xor		edx, dword ptr _long_shifts[eax * 4]
	xor		edx, edi
	mov		eax, edx
	shr		eax, 16
	movzx	ecx, al
	mov		eax, edx
	shr		eax, 8
	movzx	eax, al
	mov		ebx, dword ptr _long_shifts[ecx * 4 + 2048]
	xor		ebx, dword ptr _long_shifts[eax * 4 + 1024]
	mov		eax, edx
	shr		eax, 24
	xor		ebx, dword ptr _long_shifts[eax * 4 + 3072]
	movzx	eax, dl
	mov		edx, dword ptr [len]
	sub		edx, 24576
	mov		dword ptr [len], edx
	xor		ebx, dword ptr _long_shifts[eax * 4]
	sub		dword ptr [ebp - 4], 1
	jne		crc32_asm_longshift_start

crc32_asm_longshift_end:
	cmp		edx, 768
	jb		crc32_asm_shortshift_end
	mov		eax, -1431655765
	mul		edx
	shr		edx, 9
	mov		dword ptr [ebp - 4], edx

crc32_asm_shortshift_start:
	xor		edi, edi
	lea		ecx, dword ptr [esi + 256]
	xor		eax, eax
	npad	3

crc32_asm_shortshift_loop_start:
	crc32	ebx, dword ptr [esi]
	crc32	edi, dword ptr [esi+256]
	crc32	eax, dword ptr [esi+512]
	add		esi, 4
	cmp		esi, ecx
	jb		short crc32_asm_shortshift_loop_start

crc32_asm_shortshift_loop_end:
	mov		dword ptr [ebp - 8], eax
	add		esi, 512
	mov		eax, ebx
	shr		eax, 16
	movzx	ecx, al
	mov		eax, ebx
	shr		eax, 8
	movzx	eax, al
	mov		edx, dword ptr _short_shifts[ecx * 4 + 2048]
	xor		edx, dword ptr _short_shifts[eax * 4 + 1024]
	mov		eax, ebx
	shr		eax, 24
	xor		edx, dword ptr _short_shifts[eax * 4 + 3072]
	movzx	eax, bl
	xor		edx, dword ptr _short_shifts[eax * 4]
	xor		edx, edi
	mov		eax, edx
	shr		eax, 16
	movzx	ecx, al
	mov		eax, edx
	shr		eax, 8
	movzx	eax, al
	mov		ebx, dword ptr _short_shifts[ecx * 4 + 2048]
	xor		ebx, dword ptr _short_shifts[eax * 4 + 1024]
	mov		eax, edx
	shr		eax, 24
	xor		ebx, dword ptr _short_shifts[eax * 4 + 3072]
	movzx	eax, dl
	mov		edx, dword ptr [len]
	sub		edx, 768
	mov		dword ptr [len], edx
	xor		ebx, dword ptr _short_shifts[eax * 4]
	xor		ebx, dword ptr [ebp - 8]
	sub		dword ptr [ebp - 4], 1
	jne		crc32_asm_shortshift_start

crc32_asm_shortshift_end:
	mov		eax, edx
	mov		ecx, esi
	and		eax, 7
	sub		ecx, eax
	add		ecx, edx
	cmp		esi, ecx				;	ret = (next != end);
	jae		crc32_asm1_u32_end		;	while (ret) {

crc32_asm_u32_start:				;
	crc32	ebx, dword ptr [esi]	;		crc0 = _mm_crc32_u32((unsigned int)(crc0), *next);
	add		esi, 4					;		next += 4;
	cmp		esi, ecx				;		ret = (next != end);
	jb		crc32_asm_u32_start		;	}

crc32_asm_u32_end:					;
	test	eax, eax				;	ret = (temp != 0)
	je		crc32_asm1_u8_end
crc32_asm_u8_start:					;	while (ret) {
	lea		esi, [esi + 1]			;		next++;
	crc32	ebx, byte ptr [esi - 1]	;		crc0 = __mm_crc32_u8((unsigned int)(crc0), (unsigned char)*(next - 1);
	sub		eax, 1					;		temp--;
	jne		crc32_asm_u8_start		;	}

crc32_asm_u8_end:
	not		ebx						;	crc0 ~= crc0;
	mov		eax, ebx				;	return crc0;
	pop		edi
	pop		esi						;	// EBXの復元
	pop		ebx						;	// ESIの復元
	ret								;	// stdcallであればespの処理はしなくて大
crc32c_asm endp
end
