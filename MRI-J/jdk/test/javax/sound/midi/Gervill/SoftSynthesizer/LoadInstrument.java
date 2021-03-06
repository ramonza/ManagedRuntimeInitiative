/*
 * Copyright 2007 Sun Microsystems, Inc.  All Rights Reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 only, as
 * published by the Free Software Foundation.  Sun designates this
 * particular file as subject to the "Classpath" exception as provided
 * by Sun in the LICENSE file that accompanied this code.
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * version 2 for more details (a copy is included in the LICENSE file that
 * accompanied this code).
 *
 * You should have received a copy of the GNU General Public License version
 * 2 along with this work; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Please contact Sun Microsystems, Inc., 4150 Network Circle, Santa Clara,
 * CA 95054 USA or visit www.sun.com if you need additional information or
 * have any questions.
 */

/* @test
   @summary Test SoftSynthesizer loadAllInstrument method */

import javax.sound.midi.MidiDevice;
import javax.sound.midi.MidiUnavailableException;
import javax.sound.midi.Patch;
import javax.sound.midi.Soundbank;
import javax.sound.sampled.*;
import javax.sound.midi.MidiDevice.Info;

import com.sun.media.sound.*;

public class LoadInstrument {
	
	private static void assertEquals(Object a, Object b) throws Exception
	{
		if(!a.equals(b))
			throw new RuntimeException("assertEquals fails!");
	}	
	
	private static void assertTrue(boolean value) throws Exception
	{
		if(!value)
			throw new RuntimeException("assertTrue fails!");
	}
		
	public static void main(String[] args) throws Exception {
		AudioSynthesizer synth = new SoftSynthesizer();
		synth.openStream(null, null);
		Soundbank defsbk = synth.getDefaultSoundbank();
		if(defsbk != null)
		{
			assertTrue(synth.getLoadedInstruments().length == 0);
			synth.unloadAllInstruments(defsbk);
			SimpleSoundbank sbk = new SimpleSoundbank();
			SimpleInstrument ins = new SimpleInstrument();
			ins.setPatch(new Patch(0,1));
			sbk.addInstrument(ins);
			SimpleInstrument ins2 = new SimpleInstrument();
			ins2.setPatch(new Patch(0,2));
			sbk.addInstrument(ins2);
			synth.loadInstrument(ins2);
			assertTrue(synth.getLoadedInstruments().length == 1);
		}
		synth.close();
		
	}
}
