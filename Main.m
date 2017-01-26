clear all,close all,clc;

tic
Action = 'Calculate';

%% read configs
[configs,flags] = getConfig();
[len,wid] = size(configs);
AppScale = 0;
rs = 3000;   % Sense Resistance
cell_bit = 8;
sig_bit = 8;
rramtech = 32e-9;

%init configs
minXbarSize = 4;
maxXbarSize = 128;
minCell = 1;
maxCell = 1;
minAdPos = 1;
maxAdPos = 1;
minBtLv = 0;
maxBtLv = 16;
minRRang = 1e3;
maxRRang = 1e6;
minAdder = 0;
maxAdder = 0;
minLine = 22;
maxLine = 90;
resis_range = [minRRang,maxRRang];

% trasnlate configs
for m = 1:len
    switch configs{m,1}
        case 'Simulation_Level'
            SimLv = configs{m,2};
        case 'Target_Outputs'
            Output = configs{m,2};
        case 'Application'
            App = configs{m,2};
        case 'Action_Type'
            Action = configs{m,2};
        case 'Application_Scale'
            AppScale = str2num(configs{m,2});
        case 'Network_Scale'
            for n = 1:AppScale*2
                NetScale(n) = str2num(configs{m,n+1});
            end
        case 'Weight_Polarity'
            WeightPolar = str2num(configs{m,2});
        case 'Cell_Type'
            Cell = configs{m,2};
            minCell = str2num(Cell(1));
            maxCell = str2num(Cell(1));
        case 'RRAM_Model'
            RRAMModel = configs{m,2};
        case 'RRAM_Bit_Levels'
            minBtLv = str2num(configs{m,2});
            maxBtLv = str2num(configs{m,2});
        case 'Transistor_Tech(nm)'
            TranTech = str2num(configs{m,2});
        case 'Line_Tech'
            minLine = str2num(configs{m,2});
            maxLine = str2num(configs{m,2});
        case 'Min_Crossbar_Size'
            minXbarSize = str2num(configs{m,2});
        case 'Max_Crossbar_Size'
            maxXbarSize = str2num(configs{m,2});
        case 'Write_Method'
            WriteMth = configs{m,2};
        otherwise 
            disp(['Warning: Undefined Config "',configs{m,1},'"']);
    end
end

%% choose level
if SimLv(1) == 'e' || SimLv(1) == 'E'
    sim_level = 0;
end

if SimLv(1) == 'b' || SimLv(1) == 'B'
    sim_level = 1;
end

if SimLv(1) == 'c' || SimLv(1) == 'C'
    sim_level = 2;
end

if RRAMModel(1) == 'i' || RRAMModel(1) == 'I'
       rram_type = 0;
       
   end
   if RRAMModel(1) == 's' || RRAMModel(1) == 'S'
       rram_type = 1;
       
   end

% write_type:ideal[0],pulse[1],dcsweap[2]
   if WriteMth(1) == 'i' || WriteMth(1) == 'I'
       write_type = 0;
       
   end
   if WriteMth(1) == 'p' || WriteMth(1) == 'P'
       write_type = 1;
       select = 1;
        num_voltage = 1;
        num_pulse = 401;
        sigma = 0;
        write_parameters = [select,num_voltage,num_pulse,sigma];
   end

if sim_level ==1 ||sim_level ==2
   %% Circuit Level Simulation
%    choose model
% rram_type:ideal[0],stanford[1],peking[2]



   
%    write & read parameters
    
    write_difference = 0.1; % acceptable difference between target_r and write_result
    read_parameters = [0.2,rs];
    
%     load target
    load('target_g.mat');
    if WeightPolar == 1
    	target_resistance = 1./g;    %R=1/G 	%rram_xbar储存的是g
  	for ii = 1:AppScale-1
   	    string = ['rram_xbar(',num2str(ii),').mat']; % initial .mat file  %string是文档的名字
    	    save(string,'target_r'); 
        end
    else
        target_resistance = 1./g_pos;
        target_resistance_neg = 1./g_neg;
        for ii = 1:AppScale-1
            string = ['rram_xbar(',num2str(ii),').mat']; % initial .mat file
            save(string,'target_r'); 
            string = ['rram_xbar(',num2str(-ii),').mat']; % initial .mat file for neg xbar
            save(string,'target_r'); 
        end
    end
    
if App(1) == 'a' || App(1) == 'A'

    %    write & varify phase  %写入、确认，迭代……
    for ll = 1: AppScale-1
        for mm = 1:NetScale(ll)
            for nn = 1:NetScale(ll+1)
                resistance_target = target_resistance(mm,nn);
                if write_type == 0                                       %%write_type==0是什么意思？？？？？？？？？？？？？？？？？
                    write_parameters = resistance_target;
                end
                while(1)
                    [resistance_write,flags] = write_rram(rram_type,write_type,ll,mm,nn,write_parameters); %swtich(rram_type)
                    [resistance_read,flags] = read_rram(0,ll,mm,nn,read_parameters);
                    if abs(resistance_read -resistance_target)<= resistance_target*write_difference
                        break
                    end
                end
                if WeightPolar == 2
                    resistance_target = target_resistance_neg(mm,nn);
                    if write_type == 0
                        write_parameters = resistance_target;
                    end
                    while(1)
                        [resistance_write,flags] = write_rram(rram_type,write_type,-ll,mm,nn,write_parameters); %swtich(rram_type)
                        [resistance_read,flags] = read_rram(0,-ll,mm,nn,read_parameters);
                        if abs(resistance_read -resistance_target)<= resistance_target*write_difference
                            break
                        end
                    end
                end 
            end
        end
    end

    %    operate phase  %什么是application scale   ,,,,calculate_rram的功能是Calculate the output，所以计算输出是啥意思？我猜是模拟rram的输出……【facepalm】
    load('Input.mat');
    voltage_i(:,1) = input_voltage;
    for ll = 1:AppScale -1
        [voltage_o(:,ll),flags] = calculate_rram(ll, voltage_i(:,ll),rs);  %calculate_out = g' * input * rs;
        if WeightPolar == 2
            [voltage_o_neg(:,ll),flags] = calculate_rram(-ll, voltage_i(:,ll),rs);
        end
        for nn = 1:NetScale(ll+1)  %上一列的输出sigmoid后作为下一列的输入
            if WeightPolar == 1
                voltage_i(nn,ll+1) = sigmoid(voltage_o(nn,ll));
            else
                voltage_i(nn,ll+1) = sigmoid(voltage_o(nn,ll)-voltage_o_neg(nn,ll));
            end
        end
    end

    %   output phase  %写入输出电压
    fid = fopen('Results.txt','w');

    disp('Final Output Voltages:');
    fprintf(fid,'%s\r\n','Final Output Voltages:');
    for nn = 1:NetScale(AppScale)
        disp(['voltage(',num2str(nn),') = ',num2str(voltage_i(nn,AppScale))]);
        fprintf(fid,'%s\r\n',['voltage(',num2str(nn),') = ',num2str(voltage_i(nn,AppScale))]);
    end


end

    
   
end
if  sim_level == 0
%% Behavior level estimation
% target:area[0],power[1],latency[2]
    switch Output(1)
        case 'a'
            target = 1;
        case 'A'
            target = 1;
        case 'p'
            target = 2;
        case 'P'
            target = 2;
        case 'l'
            target = 3;
        case 'L'
            target = 3;
    end

%   load tech
    tech = TranTech;
    count = 1;
    minarea = 8e10;
    minenergy = 8e10;
    minpower = 8e10;
    minlatency = 8e10;
    minerr = 8e10;
    
    mintarget = [0,minarea,minenergy,minlatency,minerr,minpower];
%     mincount = [areacount,powercount,latencycount];
    mincount = zeros(1,3);
    if Action(1) == 'c' || Action(1) == 'C' %calculate phase
       action_type = 2;
    end
    if App(1) == 'a' || App(1) == 'A'
        application = 0;
    else
        application = 1;
    end
     
%      AAestrslt = zeros(2e4,4+7);
    for adposition = max(0,minAdPos):min(1,maxAdPos)
        for bit_level = max(0:minBtLv):min(16,maxBtLv)
%             for min_resis_range = 500:500:1e3
%                 resis_range(1) =  min_resis_range
%                 for max_resis_range = [100e3,500e3,100e4]
%                     resis_range(2) = max_resis_range
            for adderposition = max(0,minAdder):min(1,maxAdder)
                for tech = 45%[18,22,28,36,45,65,90,130];%max(22,minLine):min(22,maxLine)
%             tech = linetech;
                    for linetech = 90%[18,22,28,36,45,65,90];
%         for celltype = max(0,minCell):min(1,maxCell)
                        for celltype = 1:1
%             for pulseposition = 0:1
                            pulseposition = 0;
                
                            for xbarsize = 128:128:1024%[4,8,16,32,64,128,256]%,512,1024]
                                for read_sep = 1:1:128%xbarsize/128 : xbarsize/128 : xbarsize%8:8:xbarsize
                                    if xbarsize<minXbarSize || xbarsize > maxXbarSize
                                        disp('error:xbarsize over the limit');
                                    else
                                    input_err = zeros(1,AppScale);
                                    for netlevel = 1:AppScale %-1
                                        if bit_level ~= 0
                                            cell_bit = bit_level;
                                        end
                                        
                                        [signalsize,output_sig, input_sig,output_neighbor] = determin_sig(xbarsize,adderposition,sig_bit,cell_bit,adposition);%6666666666666666666决定带宽
                                        [netrow, netcolumn, bandwidth] = determin_net(xbarsize,NetScale(2*netlevel-1),NetScale(2*netlevel),signalsize); %66666666666666666Determine the network scale of multiple units
                    
                                        [area_u, area_flags] = unit_area_c(tech,celltype,xbarsize,adposition,adderposition,pulseposition,sig_bit,application,rramtech,read_sep);%6666666666666Calculate the whole area of an unit
                                        [latency_u,xbar_latency,adda_latency,adder_latency,decoder_latency,write_latency,read_latency, flags] = unit_latency_c(tech,celltype,xbarsize,adposition,adderposition,pulseposition,action_type,sig_bit,read_sep);   %66666666Calculate the whole latency of an unit
                                        [power_u, power_flags] = unit_power_c(tech,celltype,xbarsize,adposition,adderposition,pulseposition,action_type,sig_bit,application,resis_range,netrow,xbar_latency,adda_latency,adder_latency,decoder_latency,write_latency,read_latency,read_sep); %66666666Calculate the whole power of an unit
%                     power_u = energy_u/latency_u;
                    
%                     [area_l,input_line_area, output_line_area, flags] = line_area_c(linetech,output_sig, input_sig,output_neighbor, netrow, netcolumn,area_u,adderposition);
%                     [power_l, flags] = line_power_c(linetech, input_line_area, output_line_area,action_type);
%                     [latency_l, flags] = line_latency_c(linetech, netrow,action_type,xbarsize);
                                        area_l = 0;
                                        power_l =0;
                                        latency_l = 0;
                    
                                        [area_p, flags] = periphery_area_c(tech,xbarsize, netrow, netcolumn, adderposition,pulseposition,sig_bit,application); %外部电路面积
                    
                                        [latency_p, adders_latency,neuron_latency,pulse_latency, flags] = periphery_latency_c( tech,netrow, adderposition,pulseposition,sig_bit,application);%外部延时
                                        [power_p,adders_power,neuron_power,pulse_power, flags] = periphery_power_c(tech,xbarsize, netrow, netcolumn, adderposition,pulseposition,sig_bit,application,adders_latency,neuron_latency,pulse_latency);
                    
                                        [accuracy, d1] = accuracy_c(xbarsize,linetech,sig_bit,cell_bit,resis_range,input_err(netlevel));
                                        input_err(netlevel+1) = accuracy;
%                                       
%                     [area_r, flags] = read_area(xbarsize, netrow, netcolumn, celltype);
%                     [power_r, flags] = read_power(xbarsize, netrow, netcolumn, celltype);
%                     [latency_r, flags] = read_latency(xbarsize, netrow, netcolumn, celltype);
%                     
%                     [area_w, flags] = write_area(xbarsize, netrow, netcolumn, celltype);
%                     [power_w, flags] = write_power(xbarsize, netrow, netcolumn, celltype);
%                     [latency_w, flags] = write_latency(xbarsize, netrow, netcolumn, celltype);
                    
                    
                                        area = area_u * netrow * netcolumn + area_l + area_p ;%+ area_r + area_w;
                                        area_flags = area_flags * netrow * netcolumn;
                                        if adderposition == 0
                                            energy = power_u * latency_u * netrow * netcolumn + power_l * latency_l + power_p * latency_p;% + power_r + power_w;
                                            latency = latency_u + latency_l + latency_p;
                                            latency_multi = latency_u;
                                        else
                                            energy = power_u * latency_u * netrow * netcolumn + power_l * latency_l + power_p * latency_p;
                                            latency = latency_u * netrow + latency_l + latency_p;
                                            lantency_multi = latency_u * netrow;
                                        end
%                     power = energy/latency;
                    
                                        power_multi = power_u * netrow * netcolumn;
                                        power_flags = power_flags * netrow * netcolumn;
                                        area_multi = area_u * netrow * netcolumn;
                    
                    
                                        power = power_u  * netrow * netcolumn + power_l  + power_p ;
                                        energy = power * latency;
                                        AAestrslt(count,:) = [netlevel,area,energy,latency,power,accuracy,d1,area_multi,power_multi,latency_multi,read_sep,adposition,bit_level,adderposition,pulseposition,linetech,celltype,xbarsize];
                                        AAtestrslt(count,:) = [netlevel,output_sig, input_sig,output_neighbor,netrow, netcolumn];
                                        AAarearslt(count,:) = [netlevel,area_u,area_l,area_p,area,area_multi,area_flags];
                                        AApowerrslt(count,:) = [netlevel,power_u,power_l,power_p,power,power_multi,1e10,adders_power,neuron_power,pulse_power,power_flags];
                                        AAlatencyrslt(count,:) = [netlevel,latency_u,latency_l,latency_p,latency,latency_multi];
                                        AAenergy(count,:) = [netlevel,power_u * latency_u,power_u * latency_u * netrow * netcolumn,power_p * latency_p,energy];
                    
%                     if estrslt(count,target) < mintarget(target)
%                         mintarget(target) = estrslt(count,target);
%                         mincount(target) = count;
%                     end
%                     
%                                         for target_tt = 1:5
%                                             if d1 > 0.25
%                                                 break
%                                             else if AAestrslt(count,target_tt) < mintarget(target_tt)
%                                                 mintarget(target_tt) = AAestrslt(count,target_tt);
%                                                 mincount(target_tt) = count;
%                                                 end
%                                             end
%                                         end
                    
                                        if accuracy < minerr
                                            minerr = accuracy;
                                            err_count = count;
                                        end
                    
                                        count = count + 1;
            
                                    end
                                    
                                    
%                                     for temp_count = 1:design_space
%                                         for target_tt = 2:6
%                                             if AAAestrslt(temp_count,6)
%                                                 break
%                                             else if AAestrslt(count,target_tt) < mintarget(target_tt)
%                                                 mintarget(target_tt) = AAestrslt(count,target_tt);
%                                                 mincount(target_tt) = count;
%                                                 end
%                                             end
%                                         end
%                                     end
                                   
                                    
                                    
                                    end
                                end
                            end
                        end
                    end
                end
            end
        end
    end
    
    
    
end

layer_computation_times = [49284,49284,12100,12100,2916,2916,2916,676,676,676,144,144,144,1,1,1];
    
simulation_time = toc

design_space = (count-1)/AppScale;
                                    AAAestrslt = zeros(design_space,18);
                                    for temp_count = 1:design_space
                                        for netlevel_temp = 1:AppScale
                                            AAAestrslt(temp_count,:) = AAAestrslt(temp_count,:)+AAestrslt(((temp_count-1) * AppScale +netlevel_temp),:);
                                            AAAestrslt(temp_count,3) =AAAestrslt(temp_count,3) + AAestrslt(((temp_count-1) * AppScale +netlevel_temp),3)*(layer_computation_times(netlevel_temp)-1);
                                        end
                                        
                                        %AAAestrslt(temp_count,3) =AAestrslt(((temp_count-1) * AppScale +1),3)*layer_computation_times(1);
                                        
                                        AAAestrslt(temp_count,[4,6,7,10:end]) =AAAestrslt(temp_count,[4,6,7,10:end])/AppScale;
                                        AAAestrslt(temp_count,4) = max(AAestrslt(((temp_count-1) * AppScale+1):((temp_count-1) * AppScale+AppScale),4));
                                        for target_tt = 2:6
                                            if AAAestrslt(temp_count,6)>1
                                                break
                                            else if AAAestrslt(temp_count,target_tt) < mintarget(target_tt)
                                                mintarget(target_tt) = AAAestrslt(temp_count,target_tt);
                                                mincount(target_tt) = temp_count;
                                                end
                                            end
                                        end
                                    end

if count>2
    
    optresult = AAAestrslt(mincount(2:end),2:end);
    optresult(:,1) = optresult(:,1)*1e6;
    optresult(:,2) = optresult(:,2)*1e6;
    optresult(:,3) = optresult(:,3)*1e6;
    optresult = optresult';
end
