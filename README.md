<h1>MD5チェックツール</h1>
<h2>はじめに</h2>
<p>OpenSSLライブラリのアセンブラにより高速なハッシュ計算ができるアプリです。</p>
<p>MD5、SHA(SHA-1, SHA-2, SHA-3)はハッシュと言って、一方向関数によって割り出されたファイル固有の番号となり同じハッシュのファイルは作るのが難しいことから、ファイルが壊れていないか確認するのに使われています。</p>
<p>ただし、現在はMD5, SHA-1で同じハッシュが作れてしまうことがあるため、SHA-2(SHA-256、SHA-512)など上位のハッシュが用いられていることが多いです。</p>
<p>そう言うこともあって、一応アプリの名前から「MD5」のハッシュ計算がメインですが初回起動時に確認のメッセージボックスが出てくるので、そこから「SHA-256」に変更できるようにしています。</p>
<ul>
  <li>「いいえ」を押しても、後からオプション画面で変更できます。
</ul>
<h3>CPU拡張命令で高速計算</h3>
<p>SHA-1・SHA-224・SHA-256の計算は、CPUの拡張命令(SHAEXT) ※ でSATAのSSDは容易に超えられる速さで計算できます。</p>
<p><img src="img/md5checktool_sha-256_image1.png" width="407" height="304" border="0" alt="[ ]"></p>
<p>※SHAEXT命令が使えないPCでも、そのほかの拡張命令で可能な限り高速計算できます。<br>
環境にもよりますが200MB/秒超えるケースが多いです。</p>
<h3>効率モード (EcoQOS) に対応</h3>
<p>Microsoft Edgeブラウザに搭載されている効率モードがMD5チェックツールに対応。<br>
僅かな計算速度低下で消費電力を減らしてバッテリーを長持ちさせます。</p>
<ul>
  <li>Windows 10、11が対象でオプションから設定できます。
  <ul>
    <li>特に高速計算できるSHA-256、crc32c、xxHashなどファイルの読み込み速度が上限になりやすい場合に有効です。
  </ul>
</ul>
<p><img src="img/md5checktool_ecoqos_image1.png" width="376" height="385" border="0" alt="[ ]"></p>
<h3>幅広い動作環境</h3>
<p>最新のWindowsはもちろん、Windows 95をはじめとしたレガシーな環境に対応しています。</p>
<p><img src="img/md5checktool_xp_image2.png" width="438" height="307" border="0" alt="[ ]"></p>
<h2>動作環境</h2>
<h3>通常版 (Unicodeビルド)</h3>
<ul>
  <li><strong>Windows 2000</strong>
  <ul>
    <li>VirtualBox 7.0上のSP4で動作確認
  </ul>
  <li><strong>Windows XP</strong>
  <ul>
    <li>VirtualBox 7.0上のSP3で動作確認
  </ul>
  <li><strong>Windows XP x64 Edition</strong>
  <ul>
    <li>VirtualBox 7.0上のSP2で動作確認
  </ul>
  <li><strong>Windows Vista</strong>
  <li><strong>Windows 7</strong>
  <li><strong>Windows 8</strong>
  <li><strong>Windows 8.1</strong>
  <ul>
    <li class="warning">現在は手元になくて未確認ながら動作可能に思います。
  </ul>
  <li><strong>Windows 10</strong>
  <ul>
    <li>バージョン 2004～22H2で動作確認
    <li class="warning">上記より前のバージョンでも動作可能ですが不具合の対応はできません。
    <ul>
      <li class="warning">特に初期～2017年頃のバージョンでは、ディスプレイの拡大縮小の設定によって正常に表示できない場合があります。
    </ul>
  </ul>
  <li><strong>Windows 11</strong>
  <ul>
    <li>バージョン 22H2～23H2で動作確認
    <li class="warning">上記より前のバージョンでも動作可能ですが不具合の対応はできません。
  </ul>
</ul>
<h3>マルチバイト版</h3>
<ul>
  <li><a name="ENV-WIN95" id="ENV-WIN95"><strong>Windows 95</strong></a>
  <ul>
    <li>VirtualBox 7.0上で動作確認
    <li><a href="runtime/ReadMe.md#VC6RUNTIME">Visual C++ 6.0 SP6ランタイム</a>のインストールが必要です。
    <ul>
      <li class="information">プログラム開始エラーになり実行できません。
    </ul>
    <li><a href="runtime/ReadMe.md#MSGOTUPD">MS UI Gothic パッチ</a>の適用が必要です。
    <ul>
      <li class="information">適用しなくても実行可能ですが、ウィンドウが大きくなって見づらくなります。
      <li class="information">Office 2000など「MS UI Gothic」フォントがインストールされていれば適用する必要ありません。
    </ul>
    <li class="warning">SHA-1、SHA-2はCPU拡張命令で計算できません。
    <ul>
      <li class="warning"><s>Windows 95がSSEに対応していないことにより強制終了します。 (更新:2023年7月10日)</s>
      <li class="information">計算時にSSEを使わないようにすることで強制終了は解決できました。 (更新:2023年7月17日)
    </ul>
  </ul>
  <li><strong>Windows 98 (初期版)</strong>
  <li><strong>Windows 98 Second Edition</strong>
  <li><strong>Windows ME</strong>
  <ul>
    <li>VirtualBox 7.0上で動作確認
    <li><a href="runtime/ReadMe.me#VC6RUNTIME">Visual C++ 6.0 SP6ランタイム</a>のインストール推奨です。
    <li class="warning"><s>ファイルの読み込みが遅くなる場合があります。 (特にWindows MEは影響が大きいです)</s>
    <ul>
      <li class="information">ネステッドページングの無効化によるオーバーヘッドが原因で、<a href="https://github.com/JHRobotics/patcher9x">patcher9x</a>の適用で解決できました。 (追記:2023年7月10日)
    </ul>
  </ul>
</ul>
<h3>64ビット版</h3>
<ul>
  <li><strong>Windows XP x64 Edition</strong>
  <ul>
    <li>VirtualBox 7.0上のSP2で動作確認
  </ul>
  <li><strong>Windows Vista</strong>
  <li><strong>Windows 7</strong>
  <li><strong>Windows 8</strong>
  <li><strong>Windows 8.1</strong>
  <ul>
    <li class="warning">現在は手元になくて未確認ながら動作可能に思います。
  </ul>
  <li><strong>Windows 10</strong>
  <ul>
    <li>バージョン 2004～22H2で動作確認
    <li class="warning">上記より前のバージョンでも動作可能ですが不具合の対応はできません。
    <ul>
      <li>特に初期～2017年頃のバージョンでは、ディスプレイの拡大縮小の設定によって正常に表示できない場合があります。
    </ul>
  </ul>
  <li><strong>Windows 11</strong>
  <ul>
    <li>バージョン 22H2～23H2で動作確認
    <li class="warning">上記より前のバージョンでも動作可能ですが不具合の対応はできません。
  </ul>
</ul>
<h3>既知の不具合</h3>
<ul>
  <li>ファイルの読み込み速度に上限があります。</li>
  <ul>
    <li>環境にもよりますが5GB/秒くらいに制限されます。(XXH128の場合)<br>
    ディスクキャッシュが効いていたり、NVMe SSDから読み込む場合はご注意ください。</li>
    <li>ただし「ハッシュ速度のベンチマーク(F7)」では影響受けません。</li>
  </ul>
</ul>
