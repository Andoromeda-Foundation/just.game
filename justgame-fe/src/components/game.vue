<template>
  <section class="orders-container">

    <div id="root">
      <div class="_2FNbP">
        <div class="_3HKzV">
          <ul class="vT4_t">
<!--
            <li class="_2vjT2"><a href="https://endless.game/" target="_blank"><i></i></a></li>
-->
            <li class="m48b6">
              <div class="_3H5Ef">
                <div class="_1fto3" @click="ruleModal = true">规则</div>
              </div>
            </li>
          </ul>
          <div class="BNC_Y">
            <div class="_31aGn _1jpIb"><button data-symbol="tron" class="aVqXZ"><em>EOS</em></button></div>
            <!--<div class="_1jpIb"><button data-symbol="eth" class="_3Nh_x"><em>eth</em></button></div>
            <div class="_1jpIb"><button data-symbol="iost" class="_3BTsd"><em>iost</em></button></div>-->
          </div>
          <div class="_1AlxN">
            <div class="_3H5Ef">
              <div class="_1fto3" @click="login" v-if="account">{{account.name}}</div>
              <div class="_1fto3" @click="login" v-else>登录</div>
            </div>
            <!--<li class="_2IsC_">
              <div class="_3H5Ef">
                <div class="_1fto3">EN</div>
              </div>
            </li>-->
          </div>
        </div>
        <div class="_3ILQJ">
          <div class="_3rG6Z"><span class="_2-ZJJ"><span>{{countdown.h}}</span>:<span>{{countdown.m}}</span>:<span>{{countdown.s}}</span></span>
          </div>
        </div>
        <div class="_38Cx_">
          <div class="_2gSpK"><i class="_2TWU5"></i>
            <div>
              <p>用户收益</p>
              <p class="_26FMC _38zqW">{{convertEOS(global.pool_prize)}} <i class="icon-tron"></i></p>
              <p class="num">$ 37,306.00</p>
            </div><span class="_1654l" @click="tokenModal=true">?</span></div>
          <div class="_2gSpK"><i class="_3FOJt"></i>
            <div>
              <p>100 赢家奖励</p>
              <p class="_26FMC _2YYit">{{convertEOS(global.last100_prize)}} <i class="icon-tron"></i></p>
              <p class="num">$ 53,175.23</p>
            </div><span class="_1654l" @click="tokenModal=true">?</span></div>
          <div class="_2gSpK"><i class="_3AEek"></i>
            <div>
              <p>最后一人大奖</p>
              <p class="_26FMC _33khv">{{convertEOS(global.big_prize)}} <i class="icon-tron"></i></p>
              <p class="num">$ 5,436.02</p>
            </div><span class="_1654l" @click="tokenModal=true">?</span></div>
        </div>
        <div class="_3Nrlz">
          <div class="_1JTs3">
            <div class="_3kXmz">
              <p>收集礼物盒</p>
              <p class="_2GkKL">它们的价值只会增长！</p><span  @click="ruleModal = true">?</span></div>
            <div class="_1oJZk">
              <div></div><i>最后 100 人</i>
              <div></div>
            </div>
            <div class="_20Hb6">
              <div class="_34sLi" v-for="(item, i) in last100_player" :key="i">
                <i class="KoA2O">{{i + 1}}</i>
                <div class="_1_QYn">
                  <ul>
                    <li class="_2gMYE"><i>已购买</i></li>
                    <li class="num LzVNT">{{item.box}}</li>
                    <li><i class="icon-box"></i></li>
                  </ul>
                  <ul>
                    <li class="_2gMYE">{{item.user}}</li>
                    <li class="num LzVNT">{{playerReward(item)}}</li>
                    <li><i class="icon-tron"></i></li>
                  </ul>
                  <ul>
                    <li class="_2gMYE"><i><span class="num">+{{convertEOS(global.last100_prize)}}</span> EOS</i></li>
                    <li class="LzVNT vy-zN"><i>忠诚奖励</i></li>
                  </ul>
                </div>
              </div>
              <div class="_3V84Q">Load More</div>
            </div>
            <div class="_1keCl">
              <p>总投入量</p>
              <p class="num V4xnh">{{convertEOS(global.total_fund)}} <i class="icon-tron"></i></p>
            </div>
          </div>
          <div class="_3QV-A">
            <div class="_3Wt4G">
              <span data-name="START" :class="{'_1h8_m': tab === 'START'}" @click="switchTab($event)">开始</span>
              <span data-name="EARNINGS" :class="{'_1h8_m': tab === 'EARNINGS'}" @click="switchTab($event)">盈利</span>
              <span data-name="PROMOTE" :class="{'_1h8_m': tab === 'PROMOTE'}" @click="switchTab($event)">推广</span>
            </div>
            <!--开始-->
            <div v-if="tab === 'START'">
              <div class="_1z5hK">
                <div class="xuznu">
                  <div class="_2JBtC">
                    <p class="_3yBp5">如何开始盈利</p>
                    <p class="_27Bto">点击 ? 了解我的更多信息！</p>
                  </div>
                  <div class="_2_uun _190vv" @click="ruleModal=true"></div>
                  <div class="_1V_a8 _190vv" @click="ruleModal=true"></div>
                  <div class="_1UNBE _190vv" @click="ruleModal=true"></div>
                </div>
                <div class="_2Sdx-">
                  <p class="_3yBp5">了解如何盈利</p>
                  <p class="_3DVmJ">JUST GAME</p>
                  <p class="_27Bto">我把所有的服务收益都给到用户，就像你一样！</p>
                  <div class="_2Fyve _190vv" @click="ruleModal=true"></div>
                  <div class="_1RSxs _190vv" @click="ruleModal=true"></div>
                  <div class="_1IWqe _190vv" @click="ruleModal=true"></div>
                  <a href="https://t.me/justgamediscuss" target="_blank">
                    <div class="_77us7 _190vv"></div>
                  </a>
                </div>
              </div>
            </div>
            <!--盈利-->
            <div v-if="tab === 'EARNINGS'">
              <div class="t9Lfd">
                <div class="_2x61W"><i class="icon-box _1MDIr"></i>
                  <div class="_1J4O_"><i>{{buyNum}} 礼物盒你可以购买</i></div>
                  <div class="_3R_3Z">
                    <div class="ywlSf"><input type="text" v-model="num">
                      <div class="_1lTLB"><i class="icon-box"></i></div>
                    </div>
                    <div class="_1fBSD"><span>{{amount}}</span><i class="icon-tron"></i></div>
                  </div><button :disabled="false" @click="transfer">发送 EOS</button></div>
                <div class="AFb5V">
                  <div class="dvjGH"><i></i>
                    <p class="num">{{player.box}}</p><span>拥有的盒子数</span></div>
                  <div class="dvjGH"><i></i>
                    <p class="num">{{yourEos}}</p><span>EOS 在你的盒子里</span></div>
                </div>
                <div class="_20_oo">
                  <div class="_11eyq"><button class="_2koZ-" @click="open" :disabled="player.box <= 0"><i></i><span>打开盒子 <span class="num">(2X)</span></span></button>
                    <p>你的奖励</p><span class="num">{{yourReward}} EOS</span></div>
                  <div class="_11eyq"><button class="_2koZ-" @click="upgrade" :disabled="player.box <= 0"><i></i><span>升级盒子</span></button>
                    <p>股权增加</p><span class="num">{{stakeIncrease}} +</span></div>
                </div>
              </div>
            </div>
            <!--推广-->
            <div v-if="tab === 'PROMOTE'">
              <div class="UjuEG">
                <div class="rGAjU">{{shareUrl}}</div>
                <div class="sOtJx" @click="copyText">复制</div>
                <div class="oGArP">我的收益：<span class="num">0</span> EOS</div>
              </div>
            </div>

          </div>
        </div>
        <div class="_1Aumt">
          <div class="_36W14"><i class="B4Kz_"></i><i class="_2e2AS"></i></div>
          <a href="https://t.me/justgamediscuss" target="_blank" class="_2Q3-j"></a>
        </div>
        <div class="_3D9VK" v-show="ruleModal">
          <div class="_3Eaez"></div>
          <div class="_1nKxi"><i class="_1T42u" @click="ruleModal=false"></i>
            <div class="_20swm">
              <h3>eos justgame的游戏介绍</h3>
              <ul>
                <li>游戏的核心是礼物盒，玩家购买一个或多个盒子后，可以分得后续够买盒子的部分金额作为分红，礼物盒初始价值为 0，之后会持续升值。</li>
                <li>游戏开始倒计时 12 小时，玩家每买或开或升级一个盒子增加 30s，不超过上限 12 小时，直到最后没有人操作，计时器归 0，游戏结束，玩家开始领取收益。</li>
                <li>游戏结束后，最后 100 个购买的玩家平分奖池金额 1%，最后一个购买的玩家获得额外大奖。</li>
                <li>
                  购买盒子后可以进行如下 3 中操作：
                  <ul>
                    <li>开盒子：倒计时结束前开盒子，可以立刻获得盒子内价值 2 倍收益，资金会直接转入账户，开盒子会打开购买的全部盒子开盒子后，您可以再次购买。</li>
                    <li>升级盒子：使用当前盒子内资金购买更多盒子，即复投（前提是当前盒子价值大于购买盒子价格），当前盒子保留同时价值清空，您可以获得更多的股票分红权重。</li>
                    <li>保留盒子：倒计时结束，盒子还一直没有开，玩家可以获得当前盒子的收益。</li>
                  </ul>
                </li>
                <li>
                  玩家购买每个盒子，购买资金做如下分配：
                  <ul>
                    <li>35% 平分给之前购买的所有盒子。</li>
                    <li>35% 做为开盒子储备金，如果有玩家中途开盒子动用此资金，储备金的剩余会在游戏结束时进入奖池。</li>
                    <li>10% 进入奖池，给最后 100 个购买的玩家平分。</li>
                    <li>5% 最后一个购买的玩家的额外大奖。</li>
                    <li>5% 开发团队。</li>
                    <li>5% 作为市场推广经费。</li>
                    <li>5% 邀请人。</li>
                  </ul>
                </li>
              </ul>

              <h3>怎么玩</h3>
              <ul>
                <li>Just Game 为 EOS 版本，除了使用的钱包插件和礼物盒价格不同，其他游戏规则一致，两边游戏分别计时。</li>
                <li>
                  <ul>
                    <li>EOS 版本，您需要首先安装
                      <a href="https://chrome.google.com/webstore/detail/tronlink%EF%BC%88%E6%B3%A2%E5%AE%9D%E9%92%B1%E5%8C%85%EF%BC%89/ibnejdfjmmkpcnlpebklmnkoeoihofec"
                        target="_blank">Scatter 插件</a>，每个的礼物盒价格为 0.01 EOS； </li>
                    <!--<li>ETH 版本，您需要首先安装
                      <a href="https://chrome.google.com/webstore/detail/metamask/nkbihfbeogaeaoehlefnkodbefgpgknn" target="_blank">MetaMask 插件</a>，每个的礼物盒价格为
                      0.0025 ETH；</li>
                    <li>IOST 版本，您需要首先安装
                      <a href="https://chrome.google.com/webstore/detail/iwallet/kncchdigobghenbbaddojjnnaogfppfj" target="_blank">iWallet 插件</a>，每个的礼物盒价格为
                      25 IOST</li>-->
                  </ul>
                </li>
                <li>在“开始”查看游戏引导。</li>
                <li>在“盈利”购买盒子，之后可以完成开盒子和升级盒子等操作。</li>
                <li>在“推广”复制自己的专属链接，并传播给其他人，其他玩家使用您的链接购买礼物盒，会有 5% 的金额直接转入您的账户。</li>
              </ul>
            </div>
          </div>
        </div>
        <div class="_18z8M" v-show="tokenModal">
          <div class="Z8QHv"></div>
          <div class="_108_n"><i class="_3w9f_" @click="tokenModal=false"></i>
            <div class="_2Lyvp">
              <h3>玩家购买每个盒子，购买资金做如下分配：</h3>
              <ul>
                <li>35% 平分给之前购买的所有盒子。</li>
                <li>35% 做为开盒子储备金，如果有玩家中途开盒子动用此资金，储备金的剩余会在游戏结束时进入奖池。</li>
                <li>10% 进入奖池，给最后 100 个购买的玩家平分。</li>
                <li>5% 最后一个购买的玩家的额外大奖。</li>
                <li>5% 开发团队。</li>
                <li>5% 作为市场推广经费。</li>
                <li>5% 邀请人。</li>
              </ul>
            </div>
          </div>
        </div>
      </div>
    </div>

  </section>
</template>

<script>
  import api from '@/utils/eos';
  import APIs from '@/utils/scatter';
  import { transfer, open, upgrade, info } from '@/utils/contract'

  import "../assets/just_files/0.43a745e1.css"
  import "../assets/just_files/bundle.d0788cb5.css"

  const UNIT_PRICE = 0.01;


  export default {
    mounted() {
      //setInterval(this.fetchOrders, 1000);
      document.addEventListener('scatterLoaded', () => {
        this.login();
      });
    },

    data() {
      return {
        orders: [],
        account: null,
        tab: 'EARNINGS',
        num: 0,
        balance: 0,
        ruleModal: false,
        tokenModal: false,
        websocket: null,
        last100_player: [],
        history: [],
        global: {
          mask: 0,
          total_fund: 0,
          big_prize: 0,
          last100_prize: 0,
          pool_prize: 0
        },
        endTime: '2019-07-18T15:10:00',
        myBox: [],
        player: {
          box: 0,
          mask: 0
        },
        countdown: {
          h: '00',
          m: '00',
          s: '00'
        },
        timer: null,
        socket: null
      };
    },
    computed: {
      amount() {
        return this.mul(this.num, UNIT_PRICE);
      },
      shareUrl() {
        return `https://eos-justgame.cn/${this.account ? this.account.name : ''} `
      },
      // 你的奖励
      yourReward() {
        const { global, player } = this;
        let r = (global.mask * player.box / 10000 - player.mask) / 10000
        return r.toFixed(2)
      },
      yourEos() {
        return (this.yourReward / 2).toFixed(2);
      },
      // 股权增加
      stakeIncrease() {
        const { global, player } = this;
        let r = (global.mask * player.box / 10000 - player.mask) / global.box_value  / player.box
        return r.toFixed(2) * 100 + '%'
      },
      buyNum() {
        if (this.balance === 0) return 0;
        let b = parseFloat(this.balance.replace('eos', 0));
        return parseInt(b / UNIT_PRICE)
      }
    },
    created(){
      //页面刚进入时开启长连接
      this.initWebSocket();
      this.timer = setInterval(()=>{
        this.initCountdown()
      }, 1000);
    },
    destroyed() {
      clearInterval(this.timer);
      this.socket.close();
    },
    methods: {
      playerReward(player) {
        const { global } = this;
        let r = (global.mask * player.box / 10000 - player.mask) / 10000
        return r.toFixed(2)
      },
      convertEOS(num) {
        return (parseFloat(num) / 10000)
      },
      copyText() {
        this.$copyText(this.shareUrl).then(
            () => {
              console.log('复制成功');
              this.$message.success('复制成功');
            },
            () => {
              console.log('复制失败');
              this.$message.error('复制失败');
            }
        )
      },
      initCountdown() {
        const M1 = 60;
        const H1 = 60 * M1;
        const H12 = 12 * H1;
        let timestamp1 = Date.parse(new Date());
        let timestamp2 = Date.parse(new Date(this.endTime)) + 8 * 60 * 60 * 1000;
        let diff = (timestamp2 - timestamp1) / 1000;
        if (diff <= 0)  {
          this.countdown = {
            h: '00',
            m: '00',
            s: '00'
          }
          return;
        }
        if (diff > H12) {
          this.countdown = {
            h: '12',
            m: '00',
            s: '00'
          }
        } else {
          let h = parseInt(diff/H1).toString();
          let m = parseInt((diff % H1) / M1).toString();
          let s = (diff - h * H1 - m * M1).toString();
          this.countdown =  {
            h: h.padStart(2, '0'),
            m: m.padStart(2, '0'),
            s: s.padStart(2, '0'),
          }
        }
      },
      initWebSocket() {
        //初始化weosocket
        let socket = require('socket.io-client')('https://wss.eos-justgame.cn');
        this.socket = socket;
        socket.on('connect', function () {
          console.log('ws connect')
        });

        socket.on('disconnect', function () {
          console.log('ws disconnect')
        });

        socket.on('gamestatus', (msg) => {
          console.log(msg);
          this.history = msg.history;
          this.last100_player = msg.last100_player;
          this.global = msg.global;
          this.endTime = msg.global.end;
        });
      },
      open() {
        open({ account: this.account })
      },
      upgrade() {
        upgrade({ account: this.account })
      },
      switchTab(e) {
        this.tab = e.target.getAttribute('data-name')
      },
      transfer() {
        let urlStr = location.pathname.substr(location.pathname.lastIndexOf('/') + 1);
        let memo = urlStr ? 'buy-' + urlStr :'buy-';
        console.log(memo);
        transfer({ account: this.account, amount: this.amount, memo }).then((data) => console.log(data)).catch((error) => console.log(error))
      },
      async login() {
        console.log('login');
        const connected = await APIs.connect()
        console.log(connected);
        if (!connected) {
          this.$message.error('请先安装scatter');
          const account = APIs.account();
          console.log(account);
          APIs.suggestNetworkAsync().then(added => console.log('🛸Scatter🛸 suggest network result: ', added))
        }
        const identity = await APIs.loginScatterAsync();
        const account = identity.accounts.find(({ blockchain }) => blockchain === 'eos');
        this.account = account;
        console.log(identity, account);
        const balances = await Promise.all([
          APIs.getBalancesByContract({ symbol: 'eos', accountName: account.name })
        ])
        console.log(balances);
        const balance = balances[0][0] || '0 EOS';
        this.balance = balance;
        console.log(balance);
        await this.getMyBox();
      },
      async getMyBox() {
        let player = await info.getMyBoxAsync(this.account.name);
        if (player.length <= 0) {
          this.player = {
            box: 0,
            mask: 0
          }
        } else {
          this.player = player[0]
        }
      },
      fetchOrders() {
        api.getActions('joetothemoon', -1, -20).then(({ actions }) => {

          //console.log("xxx");
        });
      },

      dateFormat(raw) {
        return new Date(raw + 'Z').toLocaleTimeString();
      },
      mul(a, b) {
        let c = 0,
            d = a.toString(),
            e = b.toString();
        try {
          c += d.split(".")[1].length;
        } catch (f) {}
        try {
          c += e.split(".")[1].length;
        } catch (f) {}
        return Number(d.replace(".", "")) * Number(e.replace(".", "")) / Math.pow(10, c);
      },
    }
  };

</script>

<style scoped>

</style>
