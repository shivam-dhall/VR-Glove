﻿using System.Collections;
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
        private int bufferSize = 2048;
        private string tcpIpServerIP = "101.200.45.113";
        private int tcpIpServerPort = 8080;
        private Socket ClientSocket = null;
        private ManualResetEvent connectDone = new ManualResetEvent(false);
        private ManualResetEvent sendDone = new ManualResetEvent(false);
        

        private void ConnectCallback(IAsyncResult ar)
        {
            try
            {
                Socket client = (Socket)ar.AsyncState;
                client.EndConnect(ar);

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

        /// 数据接收处理,bytesRead是这次接收到的包的一部分数据size，如果没接收完，继续调用自己，直到没有数据
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

                    if (bytesRead > 0)
                    {
                        int ReadPiont = 0;
                        while (ReadPiont < bytesRead)
                        {
                            if (state.Cortrol == 0 && ReadPiont < bytesRead)
                            {
                                long bi1 = state.buffer[ReadPiont];
                                bi1 = (bi1 << 24) & 0xff000000;
                                state.packSize = bi1;
                                ReadPiont++;
                                state.Cortrol = 1;
                            }

                            if (state.Cortrol == 1 && ReadPiont < bytesRead)
                            {
                                long bi1 = state.buffer[ReadPiont];
                                bi1 = (bi1 << 16) & 0x00ff0000;
                                state.packSize = state.packSize + bi1;
                                ReadPiont++;
                                state.Cortrol = 2;
                            }

                            if (state.Cortrol == 2 && ReadPiont < bytesRead)
                            {
                                long bi1 = state.buffer[ReadPiont];
                                bi1 = (bi1 << 8) & 0x0000ff00;
                                state.packSize = state.packSize + bi1;
                                ReadPiont++;
                                state.Cortrol = 3;
                            }

                            if (state.Cortrol == 3 && ReadPiont < bytesRead)
                            {
                                long bi1 = state.buffer[ReadPiont];
                                bi1 = bi1 & 0xff;
                                state.packSize = state.packSize + bi1 - 4;
                                ReadPiont++;
                                state.Cortrol = 4;
                            }


                            if (state.Cortrol == 4 && ReadPiont < bytesRead)
                            {
                                long bi1 = state.buffer[ReadPiont];
                                bi1 = (bi1 << 24) & 0xff000000;
                                state.residualSize = bi1;
                                ReadPiont++;
                                state.Cortrol = 5;
                                state.packSize -= 1;
                            }

                            if (state.Cortrol == 5 && ReadPiont < bytesRead)
                            {
                                long bi1 = state.buffer[ReadPiont];
                                bi1 = (bi1 << 16) & 0x00ff0000;
                                state.residualSize = state.residualSize + bi1;
                                ReadPiont++;
                                state.Cortrol = 6;
                                state.packSize -= 1;
                            }

                            if (state.Cortrol == 6 && ReadPiont < bytesRead)
                            {
                                long bi1 = state.buffer[ReadPiont];
                                bi1 = (bi1 << 8) & 0x0000ff00;
                                state.residualSize = state.residualSize + bi1;
                                ReadPiont++;
                                state.Cortrol = 7;
                                state.packSize -= 1;
                            }
                            if (state.Cortrol == 7 && ReadPiont < bytesRead)
                            {
                                long bi1 = state.buffer[ReadPiont];
                                bi1 = bi1 & 0xff;
                                state.residualSize = state.residualSize + bi1;
                                state.Datastream.SetLength(0);
                                state.Datastream.Position = 0;

                                ReadPiont++;
                                state.Cortrol = 8;
                                state.packSize -= 1;
                            }

                            if (state.Cortrol == 8 && ReadPiont < bytesRead)
                            {
                                int bi1 = bytesRead - ReadPiont;
                                int bi2 = (int)(state.residualSize - state.Datastream.Length);
                                if (bi1 >= bi2)
                                {
                                    state.Datastream.Write(state.buffer, ReadPiont, bi2);
                                    ReadPiont += bi2;
                                    OnInceptEvent(new InceptEventArgs(state.Datastream, handler));
                                    state.Cortrol = 9;
                                    state.packSize -= bi2;
                                }
                                else
                                {
                                    state.Datastream.Write(state.buffer, ReadPiont, bi1);
                                    ReadPiont += bi1;
                                    state.packSize -= bi1;
                                }
                            }


                            if (state.Cortrol == 9 && ReadPiont < bytesRead)
                            {
                                int bi1 = bytesRead - ReadPiont;
                                if (bi1 < state.packSize)
                                {
                                    state.packSize = state.packSize - bi1;
                                    ReadPiont += bi1;
                                }
                                else
                                {
                                    state.Cortrol = 0;
                                    ReadPiont += (int)state.packSize;
                                }
                            }
                        }
                    }
                    else
                    {
                        throw (new Exception("读入的数据小于1bit"));
                    }
                    if (handler.Connected == true)
                    {
                        handler.BeginReceive(state.buffer, 0, bufferSize, 0, new AsyncCallback(ReceiveCallback), state);
                        Console.WriteLine("接收字节长度【{0}】", state.buffer.Length);
                        long size = state.residualSize;
                    }
                }
            }
            catch (Exception e)
            {
                OnErrorEvent(new ErrorEventArgs(e));

            }
        }

        /// 连接服务器
        public void Conn()
        {
            try
            {
                ClientSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
                IPAddress ipAddress = IPAddress.Parse(tcpIpServerIP);
                IPEndPoint remoteEP = new IPEndPoint(ipAddress, tcpIpServerPort);
                connectDone.Reset();
                ClientSocket.BeginConnect(remoteEP, new AsyncCallback(ConnectCallback), ClientSocket);
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
        public void Send(Stream Astream)
        {
            try
            {
                if (ClientSocket.Connected == false)
                {
                    throw (new Exception("没有连接服务器不可以发送信息!"));
                }
                Astream.Position = 0;
                byte[] byteData = new byte[bufferSize];
                int bi1 = (int)((Astream.Length + 8) / bufferSize);
                int bi2 = (int)Astream.Length;
                if (((Astream.Length + 8) % bufferSize) > 0)
                {
                    bi1 = bi1 + 1;
                }
                bi1 = bi1 * bufferSize;

                byteData[0] = System.Convert.ToByte(bi1 >> 24);
                byteData[1] = System.Convert.ToByte((bi1 & 0x00ff0000) >> 16);
                byteData[2] = System.Convert.ToByte((bi1 & 0x0000ff00) >> 8);
                byteData[3] = System.Convert.ToByte((bi1 & 0x000000ff));

                byteData[4] = System.Convert.ToByte(bi2 >> 24);
                byteData[5] = System.Convert.ToByte((bi2 & 0x00ff0000) >> 16);
                byteData[6] = System.Convert.ToByte((bi2 & 0x0000ff00) >> 8);
                byteData[7] = System.Convert.ToByte((bi2 & 0x000000ff));

                int n = Astream.Read(byteData, 8, byteData.Length - 8);

                while (n > 0)
                {
                    ClientSocket.BeginSend(byteData, 0, byteData.Length, 0, new AsyncCallback(SendCallback), ClientSocket);
                    Console.WriteLine("发送字节长度【{0}】", byteData.Length);
                    sendDone.WaitOne();
                    byteData = new byte[bufferSize];
                    n = Astream.Read(byteData, 0, byteData.Length);
                }
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

        /// 构造
        public MyTcpIpClient()
        {
            Conn();

            //连接完成后开始循环接收服务端返回数据
            while (true)
            {
                connectDone.WaitOne();
                StateObject Cstate = new StateObject(bufferSize, ClientSocket);
                ClientSocket.BeginReceive(Cstate.buffer, 0, bufferSize, 0,
                    new AsyncCallback(ReceiveCallback), Cstate);
                Thread.Sleep(100);
            }
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
        }

        /// 缓存
        public byte[] buffer = null;
        /// 工作插座
        public Socket workSocket = null;
        /// 数据流
        public Stream Datastream = new MemoryStream();
        /// 剩余大小
        public long residualSize = 0;
        /// 数据包大小
        public long packSize = 0;
        /// 计数器
        public int Cortrol = 0;
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
