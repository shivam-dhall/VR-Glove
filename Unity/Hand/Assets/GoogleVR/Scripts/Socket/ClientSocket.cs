using System.Collections;
using System.Collections.Generic;
using UnityEngine;

using System;
using System.IO;
using System.ComponentModel;

using System.Diagnostics;
using System.Net;
using System.Net.Sockets;
using System.Threading;

namespace clinet
{
    /// MyTcpIpClient 提供在Net TCP_IP 协议上基于消息的客户端 
    public class MyTcpIpClient : System.ComponentModel.Component
    {
        private int bufferSize = 4096;
        private string tcpIpServerIP = "101.200.45.113";
        private int tcpIpServerPort = 8080;
        private Socket ClientSocket = null;
        private ManualResetEvent connectDone = new ManualResetEvent(false);
        private ManualResetEvent sendDone = new ManualResetEvent(false);
        private bool isOnceFinished = true;
        private byte[] data = new byte[23456];
        int pos = 0;
        public bool isConnect = false;

        private void ConnectCallback(IAsyncResult ar)
        {
            try
            {
                Socket client = (Socket)ar.AsyncState;
                client.EndConnect(ar);
                //////


            }
            catch (Exception e)
            {
                OnErrorEvent(new ErrorEventArgs(e));
            }
            finally
            {
                connectDone.Set();
            }
        }
        private void SendCallback(IAsyncResult ar)
        {
            try
            {
                Socket client = (Socket)ar.AsyncState;
                int bytesSent = client.EndSend(ar);
                //Console.WriteLine("send:" + bytesSent);

            }
            catch (Exception e)
            {
                OnErrorEvent(new ErrorEventArgs(e));
            }
            finally
            {
                sendDone.Set();
            }
        }


        /// 数据接收处理//////// ,bytesRead是这次接收到的包的一部分数据size，如果没接收完，继续调用自己，直到没有数据
        private void ReceiveCallback(IAsyncResult ar)
        {
            Socket handler = null;
            try
            {
                lock (ar)
                {
                    StateObject state = (StateObject)ar.AsyncState;
                    handler = state.workSocket;
                    int bytesRead = handler.EndReceive(ar);
                    int readCnt = 0;//the length of data which has been read in this callback func
                    Index.print("receive:" + bytesRead);
                    if (bytesRead > 0)
                    {
                        if (state.cnt + bytesRead < 5)
                        {
                            for (int i = state.cnt; i < bytesRead + state.cnt; ++i)
                            {
                                state.lt_record[i] = state.buffer[i - state.cnt];
                            }

                            readCnt += bytesRead;
                        }
                        else
                        {
                            if (state.packSize == -2)
                            {
                                for (int i = state.cnt; i < 5; ++i)
                                {
                                    state.lt_record[i] = state.buffer[i - state.cnt];
                                }
                                state.packSize = (int)((state.lt_record[0] & 0xFF << 24)
                                                    | ((state.lt_record[1] & 0xFF) << 16)
                                                    | ((state.lt_record[2] & 0xFF) << 8)
                                                    | ((state.lt_record[3] & 0xFF)));
                                state.dataType = (int)state.lt_record[4];
                                readCnt += (5 - state.cnt);
                                //data = new byte[state.packSize];
                                Index.print("packsize:"+state.packSize);
                            }

                        }
                        //int pos = state.cnt - 5;
                        state.cnt += bytesRead;
                        while (readCnt < bytesRead)
                        {
                            //readCnt++;
                            data[pos++] = state.buffer[readCnt++];
                        }
                        if (state.packSize != -2)
                            state.restSize = 5 + state.packSize - state.cnt;
                        Array.Clear(state.buffer, 0, state.buffer.Length);
                    }
                    else
                    {
                        throw (new Exception("读入的数据小于1bit"));
                    }
                    if (handler.Connected == true && (state.packSize == -2 || state.restSize > 0))
                    {
                        int size = (state.restSize > bufferSize ? bufferSize : state.restSize);
                        handler.BeginReceive(state.buffer, 0, size, 0, new AsyncCallback(ReceiveCallback), state);
                    }
                    else
                    {
                        pos = 0;
                        this.isOnceFinished = true;
                        Index.SetData(data);
                    }


                }
            }
            catch (Exception e)
            {
                OnErrorEvent(new ErrorEventArgs(e));

            }
        }

        /// 连接服务器
        public void Connect()
        {
            try
            {
                ClientSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
                IPAddress ipAddress = IPAddress.Parse(tcpIpServerIP);
                IPEndPoint remoteEP = new IPEndPoint(ipAddress, tcpIpServerPort);
                connectDone.Reset();
                ClientSocket.BeginConnect(remoteEP, new AsyncCallback(ConnectCallback), ClientSocket);
                connectDone.WaitOne();
                byte[] a = new byte[2];
                a[0] = (byte)'2';
                a[1] = (byte)'\0';
                Send(a);
                ///////
                isConnect = true;
            }
            catch (Exception e)
            {
                OnErrorEvent(new ErrorEventArgs(e));
            }

        }

        /// 断开连接
        public void Close()
        {
            try
            {
                if (ClientSocket.Connected == true)
                {
                    ClientSocket.Shutdown(SocketShutdown.Both);
                    ClientSocket.Close();
                }
            }
            catch (Exception e)
            {
                OnErrorEvent(new ErrorEventArgs(e));
            }

        }

        /// 发送一个流数据
        public void Send(byte[] d)
        {
            try
            {
                if (ClientSocket.Connected == false)
                {
                    throw (new Exception("没有连接服务器不可以发送信息!"));
                }

                ClientSocket.BeginSend(d, 0, d.Length, 0, new AsyncCallback(SendCallback), ClientSocket);

            }
            catch (Exception e)
            {
                OnErrorEvent(new ErrorEventArgs(e));
            }
        }

        /// 构造
        public MyTcpIpClient(System.ComponentModel.IContainer container)
        {
            container.Add(this);
            //InitializeComponent();

            //
            // TODO: 在 InitializeComponent 调用后添加任何构造函数代码
            //
        }

        public void ReceiveData()
        {
            if (isConnect)
            {
                connectDone.WaitOne();
                
                StateObject Cstate = new StateObject(bufferSize, ClientSocket);
                if (this.isOnceFinished)
                {
                    this.isOnceFinished = false;
                    ClientSocket.BeginReceive(Cstate.buffer, 0, 93, 0,
                    new AsyncCallback(ReceiveCallback), Cstate);
                }
            }
        }

        /// 构造
        public MyTcpIpClient()
        {
            //Connect();

            ////连接完成后开始循环接收服务端返回数据
            //while (true)
            //{
            //    connectDone.WaitOne();
            //    StateObject Cstate = new StateObject(bufferSize, ClientSocket);
            //    if (this.isOnceFinished)
            //    {
            //        this.isOnceFinished = false;
            //        ClientSocket.BeginReceive(Cstate.buffer, 0, bufferSize, 0,
            //        new AsyncCallback(ReceiveCallback), Cstate);
            //    }
            //    Thread.Sleep(100);
            //}
        }

        //#region Component Designer generated code
        ///// <summary>
        ///// 设计器支持所需的方法 - 不要使用代码编辑器修改
        ///// 此方法的内容。
        ///// </summary>
        //private void InitializeComponent()
        //{
        //    Conn();
        //}
        //#endregion


        /// 要连接的服务器IP地址
        public string TcpIpServerIP
        {
            get
            {
                return tcpIpServerIP;
            }
            set
            {
                tcpIpServerIP = value;
            }
        }

        /// 要连接的服务器所使用的端口
        public int TcpIpServerPort
        {
            get
            {
                return tcpIpServerPort;
            }
            set
            {
                tcpIpServerPort = value;
            }
        }

        /// 缓冲器大小
        public int BufferSize
        {
            get
            {
                return bufferSize;
            }
            set
            {
                bufferSize = value;
            }
        }

        /// 连接的活动状态
        public bool Activ
        {
            get
            {
                if (ClientSocket == null)
                {
                    return false;
                }
                return ClientSocket.Connected;
            }
        }

        /// 接收到数据引发的事件
        public event InceptEvent Incept;
        /// 引发接收数据事件
        protected virtual void OnInceptEvent(InceptEventArgs e)
        {
            if (Incept != null)
            {
                Incept(this, e);
            }
        }

        /// 发生错误引发的事件
        public event ErrorEvent Error;

        protected virtual void OnErrorEvent(ErrorEventArgs e)
        {
            if (Error != null)
            {
                Error(this, e);
            }
        }

    }

    /// 状态对象
    public class StateObject
    {
        /// 构造
        /// <param name="bufferSize">缓存</param>
        /// <param name="WorkSocket">工作的插座</param>
        public StateObject(int bufferSize, Socket WorkSocket)
        {
            buffer = new byte[bufferSize];
            workSocket = WorkSocket;
            packSize = -2;
            restSize = -1;
            dataType = -1;
            cnt = 0;
        }

        public byte[] lt_record = new byte[5];//record length,type buffer

        /// 缓存
        public byte[] buffer = null;
        /// 工作插座
        public Socket workSocket = null;
        /// 数据流
        public Stream Datastream = new MemoryStream();


        public int packSize;/// 数据包大小//-1表示还未收数据
        public int restSize;
        public int dataType;

        public int cnt;/// 已接受的数据长度
    }

    /// 接收数据事件
    public class InceptEventArgs : EventArgs
    {
        private readonly Stream datastream;
        private readonly Socket clientSocket;
        /// 构造
        /// <param name="Astream">接收到的数据</param>
        /// <param name="ClientSocket">接收的插座</param>
        public InceptEventArgs(Stream Astream, Socket ClientSocket)
        {
            datastream = Astream;
            clientSocket = ClientSocket;
        }
        /// 接受的数据流
        public Stream Astream
        {
            get { return datastream; }
        }
        /// 接收的插座
        public Socket ClientSocket
        {
            get { return clientSocket; }
        }
    }

    /// 定义接收委托
    public delegate void InceptEvent(object sender, InceptEventArgs e);
    /// 错处事件
    public class ErrorEventArgs : EventArgs
    {
        private readonly Exception error;
        /// 构造
        /// <param name="Error">错误信息对象</param>
        public ErrorEventArgs(Exception Error)
        {
            error = Error;
        }
        /// 错误信息对象
        public Exception Error
        {
            get { return error; }
        }
    }
    /// 错误委托
    public delegate void ErrorEvent(object sender, ErrorEventArgs e);


}
