﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using SFML;

namespace GameBeak_Frontend
{
    public partial class MainWindow : Form
    {
        public MainWindow()
        {
            InitializeComponent();
        }


        private void MainWindow_FormClosing(object sender, FormClosingEventArgs e)
        {
            NativeMethods.setRunState(false);

            if(emulatorThread != null)
                emulatorThread.Abort();

            if(screenUpdateThread != null)
                screenUpdateThread.Abort();
        }

    public class NativeMethods
    {

        [DllImport("GameBeak.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool getPauseState();
        [DllImport("GameBeak.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void setPauseState(bool pauseEnabled);
        [DllImport("GameBeak.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool getRunState();
        [DllImport("GameBeak.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void setRunState(bool runEnabled);
        [DllImport("GameBeak.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr getRegisters();
        [DllImport("GameBeak.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern short getAF();
        [DllImport("GameBeak.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern short getBC();
        [DllImport("GameBeak.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern short getDE();
        [DllImport("GameBeak.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern short getHL();
        [DllImport("GameBeak.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr getStackValues();
        [DllImport("GameBeak.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern short getPC();
        [DllImport("GameBeak.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr getCurrentMemoryMap();
        [DllImport("GameBeak.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr getScreenDimensions();
        [DllImport("GameBeak.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr getScreenPixelData();
        [DllImport("GameBeak.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void setRom(byte[] rom, int romSize);
        [DllImport("GameBeak.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void setStep();
        [DllImport("GameBeak.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern byte readMemoryByte(short address);
        [DllImport("GameBeak.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void initiateEmulator();

        [DllImport("GameBeak.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void disassembleAddress(ref short address, [MarshalAs(UnmanagedType.LPStr)] StringBuilder cString, ref int stringSize);
    }
}
